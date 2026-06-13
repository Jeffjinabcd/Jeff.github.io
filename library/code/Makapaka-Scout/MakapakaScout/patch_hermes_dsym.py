
import os

file_path = "node_modules/react-native/sdks/hermes-engine/utils/build-hermes-xcode.sh"

with open(file_path, 'r') as f:
    content = f.read()

# We need to inject dsymutil execution after the copying step.
# We look for the copy command and append the dsymutil command.

target_block = """
cp -pfR \\
  "${PODS_ROOT}/hermes-engine/build/${PLATFORM_NAME}/API/hermes/hermes.framework" \\
  "${PODS_ROOT}/hermes-engine/destroot/Library/Frameworks/${platform_copy_destination}"
"""

replacement_block = """
cp -pfR \\
  "${PODS_ROOT}/hermes-engine/build/${PLATFORM_NAME}/API/hermes/hermes.framework" \\
  "${PODS_ROOT}/hermes-engine/destroot/Library/Frameworks/${platform_copy_destination}"

# ANTIGRAVITY PATCH: Generate dSYM
echo "Generating dSYM for hermes.framework"
dsymutil "${PODS_ROOT}/hermes-engine/destroot/Library/Frameworks/${platform_copy_destination}/hermes.framework/hermes" \\
  -o "${PODS_ROOT}/hermes-engine/destroot/Library/Frameworks/${platform_copy_destination}/hermes.framework.dSYM"
"""

if "dsymutil" not in content:
    if target_block.strip() in content:
        new_content = content.replace(target_block.strip(), replacement_block.strip())
        with open(file_path, 'w') as f:
            f.write(new_content)
        print("Successfully patched build-hermes-xcode.sh to generate dSYM")
    else:
        # Fallback simplistic replace if exact whitespace match fails
        fallback_target = 'cp -pfR'
        # We need to find the specific cp command. It's the last one.
        # Let's try to append to the end of the file instead, assuming variables are still valid.
        # But platform_copy_destination is defined just before headers copy? No, it's defined before framework copy.
        # Let's try strict replacement again but be careful with whitespace.
        print("Could not match exact block. Dumping content for debugging if needed.")
        print("Target block:\n" + target_block)
else:
    print("dsymutil already present in script.")
