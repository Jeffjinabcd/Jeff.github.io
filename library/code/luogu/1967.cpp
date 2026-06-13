#include <bits/stdc++.h>
using namespace std;

#define MAXN 10005

struct Edge1 {
    int x, y, dis;
} edge1[50005];  // 题目原图

struct edge {
    int to, w;
};  // 最大生成树的图

int n, m, depth[MAXN];
int f[MAXN];       // f数组表示并查集中的父节点
int fa[MAXN][21];  // fa数组表示树上的父节点  2^20=1048576
int w[MAXN][21];   // w数组表示最大载重
bool vis[MAXN];

vector<edge> g[MAXN];

bool cmp(const Edge1 &x, const Edge1 &y) {
    return x.dis > y.dis;  // 将边权从大到小排序
}

int find(int x)  // 并查集寻找父节点
{
    if (f[x] == x) {
        return x;
    }
    f[x] = find(f[x]);  // 路径压缩
    return f[x];
}

void kruskal() {
    sort(edge1 + 1, edge1 + m + 1, cmp);
    for (int i = 1; i <= n; i++) f[i] = i;  // 并查集初始化

    int cnt = 0;
    for (int i = 1; i <= m; i++) {
        int fx = find(edge1[i].x);
        int fy = find(edge1[i].y);
        if (fx != fy) {
            f[fy] = fx;
            g[edge1[i].x].push_back({edge1[i].y, edge1[i].dis});  // 无向图，双向加边
            g[edge1[i].y].push_back({edge1[i].x, edge1[i].dis});
            cnt++;
            if (cnt == n - 1) {  // 加快建树的速度
                break;
            }
        }
    }
}

void dfs(int node) {
    vis[node] = true;
    for (auto &e : g[node]) {
        int to = e.to;
        if (vis[to]) continue;
        depth[to] = depth[node] + 1;  // 计算深度
        fa[to][0] = node;             // 储存父节点
        w[to][0] = e.w;               // 储存到父节点的权值
        dfs(to);
    }
}

int lca(int x, int y) {
    if (find(x) != find(y)) return -1;  // 不连通，输出-1
    int ans = INT_MAX;
    if (depth[x] > depth[y])  // 规定y节点更深
        swap(x, y);
    // 先让y跳到x同一层
    for (int i = 20; i >= 0; i--)
        if (depth[fa[y][i]] >= depth[x]) {
            ans = min(ans, w[y][i]);  // 更新最大载重（最小边权）
            y = fa[y][i];             // 修改y位置
        }

    if (x == y)  // 如果重合，直接返回答案
        return ans;

    // x与y一起向上寻找公共祖先
    for (int i = 20; i >= 0; i--)
        if (fa[x][i] != fa[y][i])  // 只要父亲不同，就跳，父亲相同，就停
        {
            ans = min(ans, min(w[x][i], w[y][i]));  // 更新最大载重（最小边权）
            x = fa[x][i];
            y = fa[y][i];  // 修改x,y位置
        }
    ans = min(ans, min(w[x][0], w[y][0]));  // 最后一次，是没有跳上去的
    // 更新此时x,y到公共祖先最大载重，fa[x][0], fa[y][0]即为公共祖先

    return ans;
}

/*
先建一棵最大生成树，再用倍增求LCA来回答询问。
主要知识点: 并查集、最大生成树、倍增以及 LCA(最近公共祖先)
求LCA的方法是先从根节点进行搜索，求出节点深度等信息，
然后利用这些信息进行树上倍增。
*/
int main() {
    int x, y, z;
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= m; i++)  // 读题目原图的边
    {
        scanf("%d%d%d", &x, &y, &z);
        edge1[i] = {x, y, z};
    }

    kruskal();

    for (int i = 1; i <= n; i++)
        if (!vis[i])  // dfs建树
        {
            depth[i] = 1;
            fa[i][0] = i;
            w[i][0] = INT_MAX;
            dfs(i);
        }

    // LCA初始化  倍增数组
    for (int k = 1; k <= 20; k++)  // 跳的次数
        for (int i = 1; i <= n; i++) {
            int mid = fa[i][k - 1];
            fa[i][k] = fa[mid][k - 1];
            // 记录额外信息
            w[i][k] = min(w[i][k - 1], w[mid][k - 1]);
        }

    int q;
    scanf("%d", &q);
    for (int i = 1; i <= q; i++) {
        scanf("%d%d", &x, &y);
        printf("%d\n", lca(x, y));  // 回答询问
    }

    return 0;
}
