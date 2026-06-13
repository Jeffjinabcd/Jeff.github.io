
import os

file_path = "node_modules/react-native/sdks/hermes-engine/hermes-engine.podspec"

with open(file_path, 'r') as f:
    content = f.read()

# We want to change the string assignment to an Array assignment.
# Old: spec.ios.vendored_frameworks = "destroot/Library/Frameworks/ios/hermes.framework"
# New: spec.ios.vendored_frameworks = ["destroot/Library/Frameworks/ios/hermes.framework", "destroot/Library/Frameworks/ios/hermes.framework.dSYM"]

# We'll use a precise replacement approach.

patches = [
    (
        'spec.ios.vendored_frameworks = "destroot/Library/Frameworks/ios/hermes.framework"',
        'spec.ios.vendored_frameworks = ["destroot/Library/Frameworks/ios/hermes.framework", "destroot/Library/Frameworks/ios/hermes.framework.dSYM"]'
    ),
    (
        'spec.tvos.vendored_frameworks = "destroot/Library/Frameworks/tvos/hermes.framework"',
        'spec.tvos.vendored_frameworks = ["destroot/Library/Frameworks/tvos/hermes.framework", "destroot/Library/Frameworks/tvos/hermes.framework.dSYM"]'
    ),
    (
        'spec.osx.vendored_frameworks = "destroot/Library/Frameworks/macosx/hermes.framework"',
        'spec.osx.vendored_frameworks = ["destroot/Library/Frameworks/macosx/hermes.framework", "destroot/Library/Frameworks/macosx/hermes.framework.dSYM"]'
    ),
    (
        'spec.visionos.vendored_frameworks = "destroot/Library/Frameworks/xros/hermes.framework"',
        'spec.visionos.vendored_frameworks = ["destroot/Library/Frameworks/xros/hermes.framework", "destroot/Library/Frameworks/xros/hermes.framework.dSYM"]'
    )
]

new_content = content
changed = False

for old, new in patches:
    if old in new_content:
        new_content = new_content.replace(old, new)
        changed = True

if changed:
    with open(file_path, 'w') as f:
        f.write(new_content)
    print("Successfully patched hermes-engine.podspec to use Array syntax for dSYMs")
else:
    print("No changes made. Targets not found or already patched?")
