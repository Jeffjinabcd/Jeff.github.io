#include <bits/stdc++.h>
#define ll long long
using namespace std;

const int MAXN = 1e6 + 10; // Maximum possible compressed height + 1
ll c[MAXN];                // BIT array
ll left_taller[MAXN], right_taller[MAXN];
int n;

// Fenwick Tree functions using your template
int lowbit(int x) {
    return x & (-x);
}

void update(int x, int v) {
    for (int i = x; i < MAXN; i += lowbit(i))
        c[i] += v;
}

ll getSum(int x) {
    ll ans = 0;
    for (int i = x; i > 0; i -= lowbit(i))
        ans += c[i];
    return ans;
}

int main() {
    // Input
    freopen("bphoto.in", "r", stdin);
    freopen("bphoto.out", "w", stdout);

    cin >> n;
    vector<int> heights(n), ranks(n);

    for (int i = 0; i < n; i++) {
        cin >> heights[i];
    }

    // Coordinate compression
    vector<int> sorted_heights = heights;
    sort(sorted_heights.begin(), sorted_heights.end());
    unordered_map<int, int> rank_map;
    for (int i = 0; i < n; i++) {
        rank_map[sorted_heights[i]] = i + 1; // Rank starts from 1
    }
    for (int i = 0; i < n; i++) {
        ranks[i] = rank_map[heights[i]];
    }

    // Calculate left_taller using BIT
    memset(c, 0, sizeof(c));
    for (int i = 0; i < n; i++) {
        left_taller[i] = getSum(MAXN - 1) - getSum(ranks[i]);
        update(ranks[i], 1);
    }

    // Calculate right_taller using BIT
    memset(c, 0, sizeof(c));
    for (int i = n - 1; i >= 0; i--) {
        right_taller[i] = getSum(MAXN - 1) - getSum(ranks[i]);
        update(ranks[i], 1);
    }

    // Count unbalanced cows
    int unbalanced_count = 0;
    for (int i = 0; i < n; i++) {
        ll L = left_taller[i], R = right_taller[i];
        if (max(L, R) > 2 * min(L, R)) {
            unbalanced_count++;
        }
    }

    // Output result
    cout << unbalanced_count << endl;
    return 0;
}
