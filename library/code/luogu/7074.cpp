#include <bits/stdc++.h>
using namespace std;
#define LL long long
#define MIN_INF -1e18
int n, m;
LL a[1005][1005], f[1005][1005][2];

LL dfs(int x, int y, int from) {
    if (x < 1 || x > n || y < 1 || y > m) {
        return MIN_INF;
    }    
    if (f[x][y][from] != MIN_INF) {
        return f[x][y][from];
    }
    if (from) {
        f[x][y][from] = max(dfs(x - 1, y, 1), max(dfs(x, y - 1, 0), dfs(x, y - 1, 1))) + a[x][y];
    }
    else {
        f[x][y][from] = max(dfs(x + 1, y, 0), max(dfs(x, y - 1, 1), dfs(x, y - 1, 0))) + a[x][y];
    }
    return f[x][y][from];
}

/**
f[i][j][0]: 表示从一个格子左侧或者下方走到该格子的路径最大和
f[i][j][1]: 表示从一个格子左侧或者上方走到该格子的路径最大和
*/
int main() {
    // freopen("name.in","r",stdin);
    // freopen("name.out","w",stdout);
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);

    cin >> n >> m;  // n rows m cols
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= m; ++j) {
            cin >> a[i][j];
            f[i][j][0] = f[i][j][1] = MIN_INF;
        }
    f[1][1][0] = f[1][1][1] = a[1][1];
    cout << dfs(n, m, 1) << endl;

    return 0;
}