import gradio as gr
from loadimg import load_img
from transformers import AutoModelForImageSegmentation
import torch
from torchvision import transforms
import moviepy.editor as mp
from pydub import AudioSegment
from PIL import Image
import numpy as np
import os
import tempfile
import uuid
import gc  # Import garbage collector
from tqdm import tqdm  # Import tqdm for progress bar

# Enable mixed precision
torch.backends.cudnn.benchmark = True
torch.set_float32_matmul_precision("medium")
scaler = torch.cuda.amp.GradScaler()

device = "cuda" if torch.cuda.is_available() else "cpu"

# Initial transform function
transform_image = transforms.Compose(
    [
        transforms.Resize((512, 512)),  # Lower resolution to save memory
        transforms.ToTensor(),
        transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225]),
    ]
)

def update_transform(resolution):
    """Update the image transformation function with the new resolution."""
    global transform_image
    transform_image = transforms.Compose(
        [
            transforms.Resize((resolution, resolution)),  # Update resolution
            transforms.ToTensor(),
            transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225]),
        ]
    )

def initialize_models(fast_mode):
    """Initialize the models and move them to the device."""
    global birefnet, birefnet_lite
    birefnet = AutoModelForImageSegmentation.from_pretrained("ZhengPeng7/BiRefNet", trust_remote_code=True)
    birefnet.to(device)
    birefnet.eval()

    birefnet_lite = AutoModelForImageSegmentation.from_pretrained("ZhengPeng7/BiRefNet_lite", trust_remote_code=True)
    birefnet_lite.to(device)
    birefnet_lite.eval()

def clean_up():
    """Clean up GPU memory and delete models."""
    global birefnet, birefnet_lite
    if 'birefnet' in globals():
        del birefnet
    if 'birefnet_lite' in globals():
        del birefnet_lite
    torch.cuda.empty_cache()
    gc.collect()

def fn(vid, bg_type="Color", bg_image=None, bg_video=None, color="#00FF00", fps=0, input_resolution=512, batch_size=4, fast_mode=False):
    try:
        # Initialize models and clear GPU cache
        print("Initializing models...")
        initialize_models(fast_mode)
        torch.cuda.empty_cache()
        gc.collect()

        # Update the transform function with the new resolution
        update_transform(input_resolution)

        # Select the appropriate model based on fast_mode
        model = birefnet_lite if fast_mode else birefnet

        # Load the video using moviepy
        print("Loading video...")
        video = mp.VideoFileClip(vid)

        # Load original fps if fps value is equal to 0
        if fps == 0:
            fps = video.fps

        # Extract audio from the video
        audio = video.audio

        # Extract frames at the specified FPS
        print("Extracting frames...")
        frames = list(video.iter_frames(fps=fps))  # Convert to list for batch processing

        yield gr.update(visible=True), gr.update(visible=False)

        if bg_type == "Video":
            print("Loading background video...")
            background_video = mp.VideoFileClip(bg_video)
            background_frames = list(background_video.iter_frames(fps=fps))  # Convert to list

            # Ensure background frames match the number of input frames
            if len(background_frames) > len(frames):
                background_frames = background_frames[:len(frames)]
            elif len(background_frames) < len(frames):
                background_frames.extend([background_frames[-1]] * (len(frames) - len(background_frames)))
        else:
            background_frames = None

        processed_frames = []
        bg_frame_index = 0  # Initialize background frame index

        # Process frames in batches
        print("Processing frames...")
        for batch_start in tqdm(range(0, len(frames), batch_size), desc="Processing frames"):
            frame_batch = frames[batch_start:batch_start + batch_size]
            pil_images = [Image.fromarray(frame) for frame in frame_batch]

            if bg_type == "Color":
                processed_batch = process_batch(pil_images, color, model)
            elif bg_type == "Image":
                # Load the background image here
                bg_image_pil = Image.open(bg_image).convert("RGBA")
                processed_batch = process_batch(pil_images, bg_image_pil, model)
            elif bg_type == "Video":
                background_batch = background_frames[bg_frame_index:bg_frame_index + batch_size]
                bg_frame_index += batch_size
                if len(background_batch) < len(frame_batch):
                    background_batch.extend([background_batch[-1]] * (len(frame_batch) - len(background_batch)))
                processed_batch = process_batch(pil_images, [Image.fromarray(bg_frame) for bg_frame in background_batch], model)
            else:
                processed_batch = pil_images  # Default to original images if no background is selected

            processed_frames.extend(processed_batch)
            yield processed_batch[0], None

        # Create a new video from the processed frames
        print("Creating new video...")
        processed_video = mp.ImageSequenceClip([np.array(f) for f in processed_frames], fps=fps)

        # Add the original audio back to the processed video
        processed_video = processed_video.set_audio(audio)

        # Save the processed video to a temporary file
        print("Saving processed video...")
        temp_dir = "temp"
        os.makedirs(temp_dir, exist_ok=True)
        unique_filename = str(uuid.uuid4()) + ".mp4"
        temp_filepath = os.path.join(temp_dir, unique_filename)
        processed_video.write_videofile(temp_filepath, codec="libx264")

        yield gr.update(visible=False), gr.update(visible=True)
        # Return the path to the temporary file
        yield processed_batch[0], temp_filepath

    except Exception as e:
        print(f"Error: {e}")
        yield gr.update(visible=False), gr.update(visible=True)
        yield None, f"Error processing video: {e}"
    finally:
        # Clean up GPU memory and delete models
        print("Cleaning up...")
        clean_up()

def process_batch(images, bg, model):
    image_size = images[0].size
    input_images = torch.stack([transform_image(image) for image in images]).to(device)

    with torch.no_grad(), torch.cuda.amp.autocast():  # Use mixed precision
        preds = model(input_images)[-1].sigmoid().cpu()

    processed_images = []
    for i, image in enumerate(images):
        pred = preds[i].squeeze()
        mask = transforms.ToPILImage()(pred).resize(image_size)

        if isinstance(bg, str) and bg.startswith("#"):
            color_rgb = tuple(int(bg[j:j+2], 16) for j in (1, 3, 5))
            background = Image.new("RGBA", image_size, color_rgb + (255,))
        elif isinstance(bg, Image.Image):
            background = bg.convert("RGBA").resize(image_size)
        elif isinstance(bg, list) and all(isinstance(item, Image.Image) for item in bg):
            background = bg[i % len(bg)].convert("RGBA").resize(image_size)
        else:
            raise ValueError("Invalid background type")

        # Composite the image onto the background using the mask
        processed_image = Image.composite(image, background, mask)
        processed_images.append(processed_image)

    # Clear GPU cache after processing the batch
    del input_images, preds
    torch.cuda.empty_cache()
    gc.collect()

    return processed_images

with gr.Blocks(theme=gr.themes.Ocean()) as demo:
    gr.Markdown("# Video-BGR V1.0.0\n ### 可将视频背景更换为其他视频、图片、颜色")
    gr.Markdown("\n### 整合包作者：与AI同行&nbsp;&nbsp;&nbsp;&nbsp;[YouTube主页](https://www.youtube.com/@walkingwithai1996)&nbsp;&nbsp;&nbsp;&nbsp;[Bilibili主页](https://space.bilibili.com/60542591)")
    with gr.Row():
        in_video = gr.Video(label="Input Video", interactive=True)
        stream_image = gr.Image(label="Streaming Output", visible=False)
        out_video = gr.Video(label="Final Output Video")
    submit_button = gr.Button("点击更换", interactive=True)
    with gr.Row():
        fps_slider = gr.Slider(
            minimum=0,
            maximum=60,
            step=1,
            value=0,
            label="Output FPS (0 will inherit the original fps value)",
            interactive=True
        )
        bg_type = gr.Radio(["Color", "Image", "Video"], label="Background Type", value="Image", interactive=True)
        color_picker = gr.ColorPicker(label="Background Color", value="#00FF00", visible=False, interactive=True)
        bg_image = gr.Image(label="Background Image", type="filepath", visible=True, interactive=True)
        bg_video = gr.Video(label="Background Video", visible=False, interactive=True)
        
        # 新增分辨率和批处理大小滑块
        resolution_slider = gr.Slider(
            minimum=256,
            maximum=1024,
            step=32,
            value=512,
            label="Input Resolution",
            interactive=True
        )
        batch_size_slider = gr.Slider(
            minimum=1,
            maximum=16,
            step=1,
            value=4,
            label="Batch Size",
            interactive=True
        )
        fast_mode_checkbox = gr.Checkbox(label="Fast Mode (Use BiRefNet_lite)", value=False, interactive=True)

    def update_visibility(bg_type):
        if bg_type == "Color":
            return gr.update(visible=True), gr.update(visible=False), gr.update(visible=False)
        elif bg_type == "Image":
            return gr.update(visible=False), gr.update(visible=True), gr.update(visible=False)
        elif bg_type == "Video":
            return gr.update(visible=False), gr.update(visible=False), gr.update(visible=True)
        else:
            return gr.update(visible=False), gr.update(visible=False), gr.update(visible=False)

    bg_type.change(update_visibility, inputs=bg_type, outputs=[color_picker, bg_image, bg_video])

    examples = gr.Examples(
        [
            ["rickroll-2sec.mp4", "Video", None, "background.mp4"],
            ["rickroll-2sec.mp4", "Image", "images.webp", None],
            ["rickroll-2sec.mp4", "Color", None, None],
        ],
        inputs=[in_video, bg_type, bg_image, bg_video],
        outputs=[stream_image, out_video],
        fn=fn,
        cache_examples=True,
        cache_mode="eager",
    )

    submit_button.click(
        fn,
        inputs=[in_video, bg_type, bg_image, bg_video, color_picker, fps_slider, resolution_slider, batch_size_slider, fast_mode_checkbox],
        outputs=[stream_image, out_video],
    )

if __name__ == "__main__":
    demo.launch(show_error=True, inbrowser=True)