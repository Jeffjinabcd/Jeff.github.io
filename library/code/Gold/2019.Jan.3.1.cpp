#include <bits/stdc++.h>
using namespace std;

#define MAXN 10005
#define LL long long

struct point {
    int v; // 顶点编号
    LL w;  // 当前距离
};

struct edge {
    int to;  // 相邻的点
    int w;   // 通过的时间
};

struct cmp {
    bool operator()(point p1, point p2) { return p1.w > p2.w; }
};

int c[MAXN];    // 每个点初始牛的头数
int par[MAXN];  // 每个点的父节点
LL dist[MAXN];  // 到顶点1的最短路径
LL sum[MAXN];  // 经过该点的牛的总数
bool vis[MAXN];

// 原始图
vector<edge> graph[MAXN];

// 使用邻接表表示最短路径树
vector<int> sptree[MAXN];

// 跑一边DFS, 把每个顶点总共会经过多少头牛算出来
void dfs(int cur, int fa) {
    sum[cur] = c[cur];  // 本来就有的牛

    for (int to : sptree[cur]) {
        if (to == fa)  // 跳过父节点
            continue;
        dfs(to, cur);
        sum[cur] += sum[to];
    }
}

void dijkstra() {
    memset(dist, 0x3f, sizeof(dist));
    dist[1] = 0;
    priority_queue<point, vector<point>, cmp> pq;
    pq.push({1, 0});

    while (!pq.empty()) {
        point cur = pq.top();
        pq.pop();
        int u = cur.v;
        if (vis[u]) continue;
        vis[u] = true;
        for (auto e : graph[u]) {
            int v = e.to;
            LL newdis = dist[u] + e.w;
            if (newdis < dist[v]) {
                dist[v] = newdis;
                par[v] = u;
                pq.push({v, newdis});
            } else if (newdis == dist[v]) {
                // 选择字典序最小的路径
                if (u < par[v]) {
                    par[v] = u;
                }
            }
        }
    }
}

int main() {
    // freopen("shortcut.in", "r", stdin);
    // freopen("shortcut.out", "w", stdout);

    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, m, t;
    cin >> n >> m >> t;

    for (int i = 1; i <= n; i++) {
        cin >> c[i];
    }

    for (int i = 1; i <= m; i++) {
        int a, b, w;
        cin >> a >> b >> w;

        graph[a].push_back({b, w});
        graph[b].push_back({a, w});
    }

    // 跑一边Dijkstra算法求最短路
    dijkstra();

    // 生成最短路径树
    for (int i = 2; i <= n; i++) {
        sptree[i].push_back(par[i]);
        sptree[par[i]].push_back(i);
    }

    // 跑一遍dfs算出每个点会有几头奶牛经过
    dfs(1, -1);

    LL answer = 0;
    for (int i = 1; i <= n; i++) {
        // 节省的时间为sum[i] * (dist[i] - t)
        answer = max(answer, sum[i] * (dist[i] - t));
    }

    cout << answer << endl;

    return 0;
}
