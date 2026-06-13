#include <bits/stdc++.h>
using namespace std;

const int N = 1501;
int dp[N][2];  // dp[i][0]��ʾi�㲻��ʿ��ʱ����iΪ����������Ҫ������ʿ����;
               // dp[i][1]��ʾi���ʿ��ʱ����iΪ����������Ҫ������ʿ����;
vector<int> g[N];

void dfs(int cur, int fa) {
    dp[cur][1] = 1;
    for (int v : g[cur]) {
        if (v == fa) {
            continue;
        }
        dfs(v, cur);
        dp[cur][1] += min(dp[v][0], dp[v][1]);
        dp[cur][0] += dp[v][1];
    }
}

int main() {
    int n;
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        int fa, k;
        scanf("%d%d", &fa, &k);

        for (int j = 0; j < k; j++) {
            int r;
            scanf("%d", &r);
            g[fa].push_back(r);
            g[r].push_back(fa);
        }
    }

    dfs(0, -1);
    cout << min (dp[0][0], dp[0][1]);

    return 0;
}