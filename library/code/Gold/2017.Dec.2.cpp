#include <bits/stdc++.h>
using namespace std;

long long n, k;

vector<long long> g[100010];
long long MOD = 1e9 + 7;
long long a[100010];
long long dp[100010][4];

void dfs(long long cur, long long fa) {
    if (a[cur] != 0) {
        dp[cur][a[cur]] = 1;
    }
    else {
        dp[cur][1] = 1;
        dp[cur][2] = 1;
        dp[cur][3] = 1;
    }
    for (auto &e : g[cur]) {
        if (e == fa) {
            continue;
        }
        dfs(e, cur);
        dp[cur][1] = dp[cur][1] * (dp[e][2] + dp[e][3]) % MOD;
        dp[cur][2] = dp[cur][2] * (dp[e][1] + dp[e][3]) % MOD;
        dp[cur][3] = dp[cur][3] * (dp[e][1] + dp[e][2]) % MOD;
    }
}

int main () {
    freopen("barnpainting.in", "r", stdin);
    freopen("barnpainting.out", "w", stdout);
    cin >> n >> k;
    for (long long i = 0; i < n - 1; ++i) {
        long long first, second;
        cin >> first >> second;
        g[first].push_back(second);
        g[second].push_back(first);
    }
    for (long long i = 0; i < k; ++i) {
        long long first, second;
        cin >> first >> second;
        a[first] = second;
    }
    dfs(1, 0);
    cout << (dp[1][1] + dp[1][2] + dp[1][3]) % MOD;
}