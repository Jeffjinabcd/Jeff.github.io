#include<bits/stdc++.h>
using namespace std;

int b[200][2];  // b[i][0] will store LIS, b[i][1] will store LDS

int main() {
    int n;
    cin >> n;
    vector<int> heights(n);
    for (int i = 0; i < n; i++) {
        cin >> heights[i];
    }

    // Step 1: Compute LIS for each student
    for (int i = 0; i < n; i++) {
        b[i][0] = 1;  // Initialize LIS length to 1 for each element
        for (int j = 0; j < i; j++) {
            if (heights[i] > heights[j]) {
                b[i][0] = max(b[i][0], b[j][0] + 1);
            }
        }
    }

    // Step 2: Compute LDS for each student
    for (int i = n - 1; i >= 0; i--) {
        b[i][1] = 1;  // Initialize LDS length to 1 for each element
        for (int j = n - 1; j > i; j--) {
            if (heights[i] > heights[j]) {
                b[i][1] = max(b[i][1], b[j][1] + 1);
            }
        }
    }

    // Step 3: Find the largest combination of LIS and LDS
    int max_choir_length = 0;
    for (int i = 0; i < n; i++) {
        max_choir_length = max(max_choir_length, b[i][0] + b[i][1] - 1);
    }

    // Step 4: Calculate the result (students to remove)
    cout << n - max_choir_length << endl;

    return 0;
}
