
import os

file_path = "node_modules/react-native/sdks/hermes-engine/hermes-engine.podspec"

with open(file_path, 'r') as f:
    content = f.read()

# Patch 1: build-hermesc-xcode.sh
target_1 = ". ${REACT_NATIVE_PATH}/sdks/hermes-engine/utils/build-hermesc-xcode.sh #{hermesc_path} ${REACT_NATIVE_PATH}/ReactCommon/jsi"
replacement_1 = '. "${REACT_NATIVE_PATH}/sdks/hermes-engine/utils/build-hermesc-xcode.sh" "#{hermesc_path}" "${REACT_NATIVE_PATH}/ReactCommon/jsi"'

# Patch 2: build-hermes-xcode.sh
target_2 = ". ${REACT_NATIVE_PATH}/sdks/hermes-engine/utils/build-hermes-xcode.sh #{version} #{hermesc_path}/ImportHermesc.cmake ${REACT_NATIVE_PATH}/ReactCommon/jsi"
replacement_2 = '. "${REACT_NATIVE_PATH}/sdks/hermes-engine/utils/build-hermes-xcode.sh" "#{version}" "#{hermesc_path}/ImportHermesc.cmake" "${REACT_NATIVE_PATH}/ReactCommon/jsi"'

new_content = content.replace(target_1, replacement_1).replace(target_2, replacement_2)

if new_content == content:
    print("No changes made. Targets not found.")
    exit(1)

with open(file_path, 'w') as f:
    f.write(new_content)

print("Successfully patched hermes-engine.podspec")
