import subprocess

def run_test(input_str, expected_output):
    process = subprocess.Popen(['xxx.exe'], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    stdout, stderr = process.communicate(input=input_str)
    if stdout.strip() == expected_output.strip():
        print("PASS")
    else:
        print(f"FAIL. Expected: {expected_output}, Got: {stdout}")
        print(f"Stderr: {stderr}")

def compile_cpp():
    subprocess.run(['g++', 'xxx.cpp', '-o', 'xxx.exe', '-O2'], check=True)

if __name__ == "__main__":
    compile_cpp()
    
    print("Test Case 1:")
    input1 = """100 3 7
10 10
20 20
30 30
7 7
11 11
13 13
17 17
24 24
26 26
33 33"""
    run_test(input1, "22")

    print("\nTest Case 2:")
    input2 = """2 1 1
1 5
2 6"""
    run_test(input2, "3")
