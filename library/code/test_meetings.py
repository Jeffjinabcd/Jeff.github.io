import os
import subprocess

def run_test(input_str, expected_output):
    with open("meetings.in", "w") as f:
        f.write(input_str)
    
    subprocess.run(["xxx.exe"], check=True)
    
    with open("meetings.out", "r") as f:
        output = f.read().strip()
        
    if output == str(expected_output):
        print(f"PASS: Expected {expected_output}, got {output}")
    else:
        print(f"FAIL: Expected {expected_output}, got {output}")

# Sample case
sample_input = """3 5
1 1 1
2 2 -1
3 3 -1"""
print("Running Sample Case:")
run_test(sample_input, 2)

# Manual case
# N=2, L=10
# Cow 1: w=10, x=1, d=1
# Cow 2: w=20, x=2, d=-1
# Cow 1 (wt 10) moves right, reaches 10 at t=9.
# Cow 2 (wt 20) moves left, reaches 0 at t=2.
# Total weight = 30. Half = 15.
# Sorted order: Cow 1 (x=1), Cow 2 (x=2).
# Left arrivals: Cow 2 (x=2) -> t=2.
# Right arrivals: Cow 1 (x=1) -> t=10-1=9.
# Weights mapping:
# Left arrival gets weight of 1st cow (Cow 1, w=10).
# Right arrival gets weight of 2nd cow (Cow 2, w=20).
# Events: (t=2, w=10), (t=9, w=20).
# Accumulate:
# t=2, acc=10 ( < 15)
# t=9, acc=10+20=30 ( >= 15) -> T=9.
# Meetings:
# Cow 1 (right) at 1. Cow 2 (left) at 2.
# Meet at 1.5. Time = 0.5.
# 2*T = 18.
# x_j - x_i = 2 - 1 = 1 <= 18. Yes.
# Count = 1.
manual_input = """2 10
10 1 1
20 2 -1"""
print("\nRunning Manual Case:")
run_test(manual_input, 1)
