
import os

file_path = "ios/Pods/hermes-engine/lib/InternalBytecode/CMakeLists.txt"

with open(file_path, 'r') as f:
    content = f.read()

# The logic using TO_NATIVE_PATH creates double-escaped paths on macOS with Ninja/CMake.
# We replace the usage of 'internal_bytecode_sources_native_path' with 'internal_bytecode_sources'
# which relies on CMake's standard path handling (forward slashes) which works fine on Unix.
target_line = "${internal_bytecode_sources_native_path} > ${CMAKE_CURRENT_BINARY_DIR}/InternalBytecode.js"
replacement_line = "${internal_bytecode_sources} > ${CMAKE_CURRENT_BINARY_DIR}/InternalBytecode.js"

if target_line in content:
    new_content = content.replace(target_line, replacement_line)
    with open(file_path, 'w') as f:
        f.write(new_content)
    print("Successfully patched InternalBytecode/CMakeLists.txt")
else:
    print("Target line not found. Patch might have been already applied or file changed.")
