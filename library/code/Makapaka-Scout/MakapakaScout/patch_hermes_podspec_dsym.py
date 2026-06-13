
import os

file_path = "node_modules/react-native/sdks/hermes-engine/hermes-engine.podspec"

with open(file_path, 'r') as f:
    content = f.read()

# We need to append the dSYM path to the vendored_frameworks for each platform.

# iOS
ios_target = 'spec.ios.vendored_frameworks = "destroot/Library/Frameworks/ios/hermes.framework"'
ios_replacement = 'spec.ios.vendored_frameworks = "destroot/Library/Frameworks/ios/hermes.framework destroot/Library/Frameworks/ios/hermes.framework.dSYM"'

# tvOS
tvos_target = 'spec.tvos.vendored_frameworks = "destroot/Library/Frameworks/tvos/hermes.framework"'
tvos_replacement = 'spec.tvos.vendored_frameworks = "destroot/Library/Frameworks/tvos/hermes.framework destroot/Library/Frameworks/tvos/hermes.framework.dSYM"'

# OSX
osx_target = 'spec.osx.vendored_frameworks = "destroot/Library/Frameworks/macosx/hermes.framework"'
osx_replacement = 'spec.osx.vendored_frameworks = "destroot/Library/Frameworks/macosx/hermes.framework destroot/Library/Frameworks/macosx/hermes.framework.dSYM"'

# VisionOS
vision_target = 'spec.visionos.vendored_frameworks = "destroot/Library/Frameworks/xros/hermes.framework"'
vision_replacement = 'spec.visionos.vendored_frameworks = "destroot/Library/Frameworks/xros/hermes.framework destroot/Library/Frameworks/xros/hermes.framework.dSYM"'

new_content = content
changed = False

if ios_target in new_content and "dSYM" not in new_content.split(ios_target)[1].split("\n")[0]:
    new_content = new_content.replace(ios_target, ios_replacement)
    changed = True

if tvos_target in new_content and "dSYM" not in new_content.split(tvos_target)[1].split("\n")[0]:
    new_content = new_content.replace(tvos_target, tvos_replacement)
    changed = True

if osx_target in new_content and "dSYM" not in new_content.split(osx_target)[1].split("\n")[0]:
    new_content = new_content.replace(osx_target, osx_replacement)
    changed = True

if vision_target in new_content and "dSYM" not in new_content.split(vision_target)[1].split("\n")[0]:
    new_content = new_content.replace(vision_target, vision_replacement)
    changed = True

if changed:
    with open(file_path, 'w') as f:
        f.write(new_content)
    print("Successfully patched hermes-engine.podspec to vendor dSYMs")
else:
    print("No changes made. Targets not found or already patched.")
