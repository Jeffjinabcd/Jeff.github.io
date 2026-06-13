import fitz  # PyMuPDF
import random

def generate_random_highlights(input_file, output_file, highlights_per_page=4):
    doc = fitz.open(input_file)
    
    for page in doc:
        # Extract text and roughly split into sentences
        text = page.get_text("text")
        sentences = [s.strip() for s in text.split('.') if len(s.strip()) > 20]
        
        if not sentences:
            continue
            
        # Pick random sentences to highlight
        num_highlights = min(highlights_per_page, len(sentences))
        chosen_sentences = random.sample(sentences, num_highlights)
        
        for sentence in chosen_sentences:
            # Search for a chunk of the sentence to get the bounding box coordinates
            search_string = sentence[:40] 
            text_instances = page.search_for(search_string)
            
            # Draw the highlight annotation
            for inst in text_instances:
                page.add_highlight_annot(inst)
                
    doc.save(output_file)
    print(f"Generated random highlights! Saved to {output_file}")

# Set your file names here
# Set your file names here. Make sure "2.pdf" is in the same folder as this script.
generate_random_highlights("2.pdf", "2_highlighted.pdf", highlights_per_page=5)
