#include <bits/stdc++.h>
using namespace std;

#define MAXN 100001
#define MAXK 21

int gestures[MAXN];

int dp[MAXN][MAXK][4];  // dp[i][j][k] 前i回合最多换j次出的哪种手势的最大值

inline int check(int x, int y) {
    if (x == 1 && y == 2) return 1;
    if (x == 2 && y == 3) return 1;
    if (x == 3 && y == 1) return 1;
    return 0;
}

int main() {
    // freopen("hps.in", "r", stdin);
    // freopen("hps.out", "w", stdout);

    int n, k;
    cin >> n >> k;

    char c;
    for (int i = 1; i <= n; i++) {
        cin >> c;
        if (c == 'H') gestures[i] = 1;
        if (c == 'S') gestures[i] = 2;
        if (c == 'P') gestures[i] = 3;
    }

    for (int i = 1; i <= n; i++) {
        dp[i][0][1] = dp[i - 1][0][1] + check(1, gestures[i]);
        dp[i][0][2] = dp[i - 1][0][2] + check(2, gestures[i]);
        dp[i][0][3] = dp[i - 1][0][3] + check(3, gestures[i]);
    }  // 边界条件

    for (int i = 1; i <= n; i++) {      // 前i局
        for (int j = 1; j <= k; j++) {  // 最多变了几次
            dp[i][j][1] = max(dp[i - 1][j][1], max(dp[i - 1][j - 1][2], dp[i - 1][j - 1][3])) +
                          check(1, gestures[i]);
            dp[i][j][2] = max(dp[i - 1][j][2], max(dp[i - 1][j - 1][1], dp[i - 1][j - 1][3])) +
                          check(2, gestures[i]);
            dp[i][j][3] = max(dp[i - 1][j][3], max(dp[i - 1][j - 1][1], dp[i - 1][j - 1][2])) +
                          check(3, gestures[i]);
        }
    }

    cout << max(dp[n][k][1], max(dp[n][k][2], dp[n][k][3])) << endl;

    return 0;
}
