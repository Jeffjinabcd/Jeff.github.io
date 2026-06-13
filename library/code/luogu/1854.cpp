// #include <bits/stdc++.h>
#include <iostream>
#include <algorithm>
#include <climits>
using namespace std;

int a[101][101];
int dp[101][101];

int main() {
    // freopen("template.in", "r", stdin);
	// freopen("template.out", "w", stdout);
    ios::sync_with_stdio(false);
    cin.tie(0);

    int f, v;
    cin >> f >> v;

    for (int i = 1; i <= f; i++) {
        for (int j = 1; j <= v; j++) {
            cin >> a[i][j];
            dp[i][j] = INT_MIN / 2;
        }
        dp[1][i] = a[1][i];
    }

    dp[0][0] = 0;
    for (int i = 2; i <= f; i++) {
        for (int j = i; j <= v - f + i; j++) {
            for (int k = i; k <= j; k++) {
                dp[i][j] = max(dp[i][j], dp[i - 1][k] + a[i][j]);
            }
        }
    }

    int maxn = INT_MIN;
    int bJ = 0;
    for (int j = f; j <= v; j++) {
        if (dp[f][j] > maxn) {
            maxn = dp[f][j];
            bJ = j;
        }
    }

    cout << maxn << "\n";

    int placement[101];
    int currJ = bJ;
    for (int i = f; i >= 1; i--) {
        placement[i] = currJ;
        for (int k = i - 1; k < currJ; k++) {
            if (dp[i - 1][k] + a[i][currJ] == dp[i][currJ]) {
                currJ = k;
                break;
            }
        }
    }

    cout << placement[1];
    for (int i = 2; i <= f; i++) {
        cout << " " << placement[i];
    }
    cout << "\n";

    return 0;
}
