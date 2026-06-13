#include <bits/stdc++.h>
using namespace std;

int n, m;
int matrix[505][505], vis[505][505];
int ans[505];
int f[505];  // f[i]：覆盖前i个点最少需要多少座蓄水厂

struct segment {
    int l, r;
};

const int DIR[4][2] = {{1, 0}, {-1, 0}, {0, -1}, {0, 1}};

segment cover[505];

// x：行, y：列，point: 对应的起点
void dfs(int x, int y, int point) {
    if (vis[x][y] == 1) return;
    vis[x][y] = 1;  //记住搜过的点
    if (x == n) {
        ans[y] = 1;
        // 到最后一排更新cover
        cover[point].l = min(cover[point].l, y);
        cover[point].r = max(cover[point].r, y);
        // 这里不能return
    }
    for (int i = 0; i < 4; i++) {
        int nx = x + DIR[i][0];
        int ny = y + DIR[i][1];
        // 判断是否出界
        if (nx > n || nx < 1 || ny > m || ny < 1) {
            continue;
        }
        if (matrix[nx][ny] < matrix[x][y]) {
            dfs(nx, ny, point);
        }
    }
}

/*
如果存在解，每个起点的覆盖区间必定连续。将问题转化为区间覆盖问题。
*/
int main() {
    scanf("%d %d", &n, &m);
    for (int i = 1; i <= m; i++) {
        cover[i].l = INT_MAX;
        f[i] = INT_MAX;
    }

    f[0] = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            scanf("%d", &matrix[i][j]);
        }
    }

    for (int i = 1; i <= m; i++) {
        if (matrix[1][i] >= matrix[1][i - 1] &&
            matrix[1][i] >= matrix[1][i + 1]) {  //优化
            memset(vis, 0, sizeof(vis));         // 注意每次清0
            dfs(1, i, i);
        }
    }

    int cnt = 0;
    for (int i = 1; i <= m; i++)
        if (!ans[i]) cnt++;

    if (cnt)
        printf("0\n%d", cnt);
    else {
        cout << 1 << endl;
        // 找最少的点
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= m; j++) {
                if (i >= cover[j].l && i <= cover[j].r)
                    f[i] = min(f[i], f[cover[j].l - 1] + 1);
            }
        }
        cout << f[m] << endl;
    }

    return 0;
}
