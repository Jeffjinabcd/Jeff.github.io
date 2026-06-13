#include <bits/stdc++.h>
using namespace std;

const int INF = 0x3f3f3f3f;
int N, M, ans;

struct edge {
    int u, v, cost, flow;
} e[1000];

bool cmp(const edge &a, const edge &b) {
    return a.flow > b.flow;
}

vector<pair<int, int> > graph[1001];
int dis[1001], vis[1001];

struct node {
    int no, dis;
    bool operator<(const node &b) const {
        return dis > b.dis;
    }
};

inline void Dijkstra() {
    priority_queue<node> q;
    memset(dis, 0x3f, sizeof(dis));
    memset(vis, 0, sizeof(vis));
    dis[1] = 0;
    q.push({1, 0});  //{id, dis}
    while (!q.empty()) {
        node top = q.top();
        q.pop();
        if (vis[top.no]) continue;
        vis[top.no] = 1;
        vector<pair<int, int> > vec = graph[top.no];
        for (auto p : vec) {
            if (dis[p.first] > dis[top.no] + p.second) {
                dis[p.first] = dis[top.no] + p.second;
                q.push({p.first, dis[p.first]});
            }
        }
    }
}

int main() {
    // freopen("pump.in", "r", stdin);
    // freopen("pump.out", "w", stdout);
    scanf("%d%d", &N, &M);
    for (int i = 0; i < M; i++)
        scanf("%d%d%d%d", &e[i].u, &e[i].v, &e[i].cost, &e[i].flow);
        
    sort(e, e + M, cmp);
    for (int i = 0; i < M; i++) {
        graph[e[i].u].push_back({e[i].v, e[i].cost});
        graph[e[i].v].push_back({e[i].u, e[i].cost});
        Dijkstra();
        if (dis[N] != INF) ans = max(ans, e[i].flow * 1000000 / dis[N]);
    }
    printf("%d\n", ans);

    return 0;
}
