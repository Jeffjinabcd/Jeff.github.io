#include <bits/stdc++.h>
using namespace std;

const int maxn = 1501;
int n;
int dp[maxn][3], w[maxn];  // ��ͬ�ĵ��Ӧ��Ȩֵ
// dp[i][0]���ڵ㸲�� dp[i][1]�ӽڵ㸲�� dp[i][2]�Լ�����

vector<int> g[maxn];

void dfs(int cur, int fa) {  // cur:��Ҫ�������ӽڵ㣬fa:�丸�ڵ�
    int d = INT_MAX;
    for (int son : g[cur]) {
        if (son == fa) continue;
        dfs(son, cur);
        dp[cur][0] += min(dp[son][2], dp[son][1]);
        dp[cur][1] += min(dp[son][2], dp[son][1]);
        d = min(d, dp[son][2] - min(dp[son][1], dp[son][2]));
        dp[cur][2] += min(dp[son][1], min(dp[son][2], dp[son][0]));
    }
    dp[cur][1] += d;
    dp[cur][2] += w[cur];
}

int main() {
    ios::sync_with_stdio(0);
    cin >> n;
    for (int i = 0; i < n; i++) {
        int x, k, m, r;
        // x:�ýڵ����, k:���ѣ�m:�ӽڵ����, r�ӽڵ����
        cin >> x >> k >> m;

        w[x] = k;  //��¼Ȩֵ

        for (int j = 0; j < m; j++) {
            cin >> r;
            g[x].push_back(r);
            g[r].push_back(x);
        }
    }

    dfs(1, 0);
    cout << min(dp[1][1], dp[1][2]) << endl;
    return 0;
}