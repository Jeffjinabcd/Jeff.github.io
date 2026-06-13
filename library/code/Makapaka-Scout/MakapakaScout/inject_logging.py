
import os

script_path = "node_modules/react-native/sdks/hermes-engine/utils/build-hermes-xcode.sh"

with open(script_path, 'r') as f:
    content = f.read()

# Logging injection
log_cmd = 'exec > /tmp/hermes_build_log.txt 2>&1\nprintenv\n'
if '/tmp/hermes_build_log.txt' not in content:
    content = content.replace('set -x -e', 'set -x -e\n' + log_cmd)
    
    with open(script_path, 'w') as f:
        f.write(content)
    print("Injected logging into build script.")
else:
    print("Logging already injected.")
