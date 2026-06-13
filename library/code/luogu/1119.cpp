#include <bits/stdc++.h>
#define maxn 205
#define maxq 50005
#define INF 0x3f3f3f3f

using namespace std;

int n, m, q, t[maxn];
int dist[maxn][maxn];

/*
Floyd算法的本质是动态规划，其转移方程为：
f[k][i][j] = min(f[k-1][i][j], f([k-1][i][k]+f[k-1][k][j])
*/
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);

    memset(dist, 0x3f, sizeof(dist));

    cin >> n >> m;
    for (int i = 0; i < n; i++) cin >> t[i];
    int a, b, c;
    for (int i = 1; i <= m; i++) {
        cin >> a >> b >> c;
        dist[a][b] = c;
        dist[b][a] = c;
    }
    for (int i = 0; i < n; i++) dist[i][i] = 0;

    int k = 0;  // 意为当前的最短路是仅通过前k个点的最短路
    cin >> q;
    for (int i = 1; i <= q; i++) {
        cin >> a >> b >> c;
        if(t[a] > c || t[b] > c) {
            cout << -1 << endl;
            continue;
        }
        for (; k < n && t[k] <= c; k++) {
            for (int x = 0; x < n; x++)
                for (int y = 0; y < n; y++)
                    dist[x][y] = min(dist[x][y], dist[x][k] + dist[k][y]);
        }
        if (dist[a][b] == INF)
            cout << -1 << endl;
        else
            cout << dist[a][b] << endl;
    }
    return 0;
}
