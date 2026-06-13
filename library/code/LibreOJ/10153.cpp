#include <bits/stdc++.h>
using namespace std;

const int N = 110;

int n, q, son[N][2], a[N], g[N][N], f[N][N];
// 状态f[i][j]表示以i为根的树上保留j个节点的最大权值和

void dfs(int u) {
    for (int i = 1; i <= n && !son[u][0]; ++i) {
        if (g[u][i] != -1) {
            son[u][0] = i;
            a[i] = g[u][i];
            g[u][i] = g[i][u] = -1;
            dfs(i);
        }
    }
    for (int i = 1; i <= n && !son[u][1]; ++i) {
        if (g[u][i] != -1) {
            son[u][1] = i;
            a[i] = g[u][i];
            g[u][i] = g[i][u] = -1;
            dfs(i);
        }
    }

    return;
}

int dp(int u, int x) {
    if (f[u][x]) {
        return f[u][x];
    }
    if (x == 0) {
        return 0; 
    }
    if (!son[u][0] && !son[u][1]) {
        return a[u];
    }
    for (int i = 0; i < x; ++i) {
        f[u][x] = max (f[u][x], dp(son[u][0], i) + dp(son[u][1], x - 1 - i) + a[u]);
    }
    return f[u][x];
}

int main() {
    scanf("%d%d", &n, &q);
    memset(g, -1, sizeof(g));

    for (int i = 1, u, v, w; i < n; i++) {
        scanf("%d%d%d", &u, &v, &w);
        g[u][v] = g[v][u] = w;
    }

    dfs(1);
    printf("%d", dp(1, q + 1));
    return 0;
}