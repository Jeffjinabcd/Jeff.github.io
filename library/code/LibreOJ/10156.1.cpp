#include <bits/stdc++.h>
using namespace std;

const int N = 1501;
int dp [N][2];
vector <int> g[N];

void dfs(int cur, int fa) {
    dp[cur][1] = 1;
    for (auto &e : g[cur]) {
        if (e == fa) continue;
        dfs(e, cur);
        dp[cur][0] += dp[e][1];
        dp[cur][1] += min(dp[e][0], dp[e][1]);
    }
}

int main() {
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        int fa, k;
        cin >> fa >> k;
        for (int j = 0; j < k; j++) {
            int son;
            cin >> son;
            g[fa].push_back(son);
            g[son].push_back(fa);
        }
    }
    dfs(0, -1);
    cout << min(dp[0][0], dp[0][1]) << endl;
}