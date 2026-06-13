#include <bits/stdc++.h>
#define LL long long
using namespace std;

const int N = 1e5 + 5;
const int MOD = 1e9 + 7;
int n, k;

vector<int> g[N];

int colr[N];
LL dp[N][4];

// LL dfs(int curid, int curc, int fa, int fac) {
//     // TODO
// }

void dfs(int cur, int fa) {
    if (colr[cur]) {
        dp[cur][colr[cur]] = 1;
    }
    else {
        dp[cur][1] = dp[cur][2] = dp[cur][3] = 1;
    }
    for (auto &e:g[cur]) {
        if (e == fa) continue;
        dfs(e, cur);
        dp[cur][1] = dp[cur][1] * (dp[e][2] + dp[e][3]) % MOD;
        dp[cur][2] = dp[cur][2] * (dp[e][1] + dp[e][3]) % MOD;
        dp[cur][3] = dp[cur][3] * (dp[e][1] + dp[e][2]) % MOD;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    freopen("barnpainting.in", "r", stdin);
    freopen("barnpainting.out", "w", stdout);
    cin >> n >> k;
    for (int i = 1; i <= n - 1; i++) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    for (int i = 1; i <= k; i++) {
        int id, c;
        cin >> id >> c;
        colr[id] = c;
    }

    // int ans = (dfs(1, 1, 0, 0) + dfs(1, 2, 0, 0)) % MOD;
    // ans = (ans + dfs(1, 3, 0, 0)) % MOD;
    
    dfs(1, 0);
    LL ans = dp[1][1] + dp[1][2] + dp[1][3];
    cout << ans % MOD << endl;

    return 0;
}
