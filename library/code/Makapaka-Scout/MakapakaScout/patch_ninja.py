
import os

scripts = [
    "node_modules/react-native/sdks/hermes-engine/utils/build-hermes-xcode.sh",
    "node_modules/react-native/sdks/hermes-engine/utils/build-hermesc-xcode.sh"
]

for script in scripts:
    with open(script, 'r') as f:
        content = f.read()

    # Inject PATH
    if '/opt/homebrew/bin' not in content:
        content = content.replace('set -x -e', 'set -x -e\nexport PATH="/opt/homebrew/bin:$PATH"')

    # Inject Ninja generator
    # We look for the CMAKE_BINARY call that configures the project (uses -S)
    # We avoid replacing the build command (uses --build) although -G Ninja is harmless there, it's invalid.
    
    # We search for the pattern '"$CMAKE_BINARY" \' which usually starts the configure command
    # And we verify it is followed by -S or similar logic in next lines.
    
    # Simple heuristic: Replace '"$CMAKE_BINARY" \' with '"$CMAKE_BINARY" -G Ninja \'
    # BUT! check if it is already there.
    
    if '-G Ninja' not in content:
        # This replaces the start of the multiline cmake command
        content = content.replace('"$CMAKE_BINARY" \\\n  -S', '"$CMAKE_BINARY" -G Ninja \\\n  -S')
        
        # Checking for single line execution in hermesc script
        # "$CMAKE_BINARY" -S "${PODS_ROOT}/hermes-engine" -B "$hermesc_dir_path" ...
        content = content.replace('"$CMAKE_BINARY" -S', '"$CMAKE_BINARY" -G Ninja -S')

    with open(script, 'w') as f:
        f.write(content)
        print(f"Patched {script}")

print("Successfully injected Ninja generator.")
