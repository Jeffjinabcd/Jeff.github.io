import sys
import os

patches = [
    {
        "name": "Fix get-app-config-ios.sh path quoting in Pods",
        "path": "ios/Pods/Pods.xcodeproj/project.pbxproj",
        "target": 'bash -l -c \\"$PODS_TARGET_SRCROOT/../scripts/get-app-config-ios.sh\\"',
        "replacement": '\\"$PODS_TARGET_SRCROOT/../scripts/get-app-config-ios.sh\\"'
    },
    {
        "name": "Fix react-native-xcode.sh path quoting in Main Project",
        "path": "ios/MakapakaScout.xcodeproj/project.pbxproj",
        "target": '`\\"$NODE_BINARY\\" --print \\"require(\'path\').dirname(require.resolve(\'react-native/package.json\')) + \'/scripts/react-native-xcode.sh\'\\"`',
        "replacement": '\\"`\\"$NODE_BINARY\\" --print \\"require(\'path\').dirname(require.resolve(\'react-native/package.json\')) + \'/scripts/react-native-xcode.sh\'\\"`\\"'
    }
]

def apply_patch(patch):
    path = patch["path"]
    name = patch["name"]
    target = patch["target"]
    replacement = patch["replacement"]

    if not os.path.exists(path):
        print(f"[{name}] File not found: {path}")
        return

    try:
        with open(path, "r") as f:
            content = f.read()

        if replacement in content:
             print(f"[{name}] Already patched.")
             return

        if target not in content:
            print(f"[{name}] Target string not found! (May depend on exact formatting)")
            # Optional: print snippet for debugging
            # print(f"Searching for: {target}")
            return
            
        new_content = content.replace(target, replacement)

        with open(path, "w") as f:
            f.write(new_content)
        print(f"[{name}] Successfully applied patch.")

    except Exception as e:
        print(f"[{name}] Error: {e}")

if __name__ == "__main__":
    print("Starting project patching...")
    for patch in patches:
        apply_patch(patch)
    print("Done.")
