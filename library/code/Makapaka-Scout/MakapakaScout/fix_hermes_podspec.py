
import os

file_path = "node_modules/react-native/sdks/hermes-engine/hermes-engine.podspec"

with open(file_path, 'r') as f:
    content = f.read()

# The broken lines look like:
# spec.ios.vendored_frameworks = "destroot/Library/Frameworks/ios/hermes.framework destroot/Library/Frameworks/ios/hermes.framework.dSYM"

# We want to revert them to:
# spec.ios.vendored_frameworks = "destroot/Library/Frameworks/ios/hermes.framework"

# And we will blindly add preserve_paths for the dSYMs to ensure they are kept.
# But "preserve_paths" is already set to "**/*.*" in the podspec (line 42).
#   spec.preserve_paths      = '**/*.*'
# So we don't need to add anything extra for preservation if the dSYM is in the source tree.

# Fix the 4 platform lines.
fixes = {
    'spec.ios.vendored_frameworks = "destroot/Library/Frameworks/ios/hermes.framework destroot/Library/Frameworks/ios/hermes.framework.dSYM"':
    'spec.ios.vendored_frameworks = "destroot/Library/Frameworks/ios/hermes.framework"',

    'spec.tvos.vendored_frameworks = "destroot/Library/Frameworks/tvos/hermes.framework destroot/Library/Frameworks/tvos/hermes.framework.dSYM"':
    'spec.tvos.vendored_frameworks = "destroot/Library/Frameworks/tvos/hermes.framework"',

    'spec.osx.vendored_frameworks = "destroot/Library/Frameworks/macosx/hermes.framework destroot/Library/Frameworks/macosx/hermes.framework.dSYM"':
    'spec.osx.vendored_frameworks = "destroot/Library/Frameworks/macosx/hermes.framework"',

    'spec.visionos.vendored_frameworks = "destroot/Library/Frameworks/xros/hermes.framework destroot/Library/Frameworks/xros/hermes.framework.dSYM"':
    'spec.visionos.vendored_frameworks = "destroot/Library/Frameworks/xros/hermes.framework"'
}

new_content = content
changed = False

for broken, fixed in fixes.items():
    if broken in new_content:
        new_content = new_content.replace(broken, fixed)
        changed = True

if changed:
    with open(file_path, 'w') as f:
        f.write(new_content)
    print("Successfully reverted broken vendored_frameworks paths.")
else:
    print("No broken paths found. File might be already fixed or different than expected.")
