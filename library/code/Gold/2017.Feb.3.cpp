#include <bits/stdc++.h>
using namespace std;

const int MAXN = 100000;
long long c[MAXN + 1]; // BIT array to keep track of active cows

// BIT functions
int lowbit(int x) {
    return x & (-x);
}

void update(int x, int v) {
    for (int i = x; i <= MAXN; i += lowbit(i))
        c[i] += v;
}

long long getSum(int x) {
    long long ans = 0;
    for (int i = x; i > 0; i -= lowbit(i))
        ans += c[i];
    return ans;
}

int main() {
    int N;
    cin >> N;

    vector<int> positions(2 * N); // Sequence of cow IDs
    vector<int> entry(N + 1, -1); // Entry positions for each cow (1-indexed)

    // Read input sequence
    for (int i = 0; i < 2 * N; i++) {
        cin >> positions[i];
    }

    long long crossing_pairs = 0;

    // Process the sequence
    for (int i = 0; i < 2 * N; i++) {
        int cow = positions[i];

        if (entry[cow] == -1) {
            // First occurrence: Mark entry point
            entry[cow] = i + 1; // Convert to 1-indexed for BIT
        } else {
            // Second occurrence: cow exits
            int entry_position = entry[cow];

            // Count the number of cows whose entry point is between entry_position and current exit
            long long count_between = getSum(i + 1) - getSum(entry_position);
            crossing_pairs += count_between;

            // Remove this cow's entry from BIT (it has exited now)
            update(entry_position, -1);
        }

        // Mark the current position as active in BIT (cow has entered)
        update(i + 1, 1);
    }

    // Output the result
    cout << crossing_pairs << endl;

    return 0;
}
