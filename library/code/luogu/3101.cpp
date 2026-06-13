#include <bits/stdc++.h>
using namespace std;
#define LL long long

LL dir[2][2] = {{1, 0}, {0, 1}};
LL n, m, t;
LL cnt, tot;
LL ans;

struct edge {
    LL x, y, dis;
} e[250001 * 2];

LL f[505][505];
LL fa[250005], size[250005];
LL num[505][505], start[250005]; // start[i]记录连通块中起始点的个数

LL find(LL x) {
    if (x == fa[x]) {
        return x;
    }

    fa[x] = find(fa[x]);
    return fa[x];
}

bool cmp(edge x, edge y) {
    return x.dis < y.dis;
}

int main() {
    // freopen("skilevel.in", "r", stdin);
    // freopen("skilevel.out", "w", stdout);

    scanf("%lld%lld%lld", &n, &m, &t);
    tot = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            scanf("%lld", &f[i][j]);
            tot++;
            num[i][j] = tot;
        }
    }

    // 按边存图
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            int flag;
            scanf("%d", &flag);
            if (flag) start[num[i][j]] = 1;
            for (int k = 0; k < 2; k++) {
                int tx = i + dir[k][0], ty = j + dir[k][1];
                if (tx < n + 1 && ty < m + 1)
                    e[++cnt] = {num[i][j], num[tx][ty], abs(f[i][j] - f[tx][ty])};
            }
        }
    }
    sort(e + 1, e + 1 + cnt, cmp);  //排序

    for (int i = 1; i <= 250004; i++) {
        size[i] = 1, fa[i] = i;
    }

    for (int i = 1; i <= cnt; i++)  //不断加边
    {
        int x = e[i].x, y = e[i].y;
        int fx = find(x), fy = find(y);
        if (fx == fy) continue;
        if (size[fx] + size[fy] >= t) {
            if (size[fx] < t) ans += e[i].dis * start[fx];
            if (size[fy] < t) ans += e[i].dis * start[fy];
        }
        if (size[fx] > size[fy]) swap(fx, fy); // 按秩合并

        fa[fx] = fy;
        size[fy] += size[fx];
        start[fy] += start[fx];
    }
    printf("%lld", ans);  //输出答案
    return 0;
}
