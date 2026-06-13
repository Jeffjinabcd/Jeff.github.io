#include <bits/stdc++.h>
using namespace std;
#define ll long long

const int MAXN = 50010;
int n, m, k;
ll dis[MAXN], a[MAXN], ans[MAXN];

struct edge {
    int to;
    ll cost;
};
vector<edge> G[MAXN];

void dijkstra(int s) {
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> q;

    memset(dis, 0x3f, sizeof(dis));
    dis[s] = 0;
    q.push({0, s});
    while (!q.empty()) {
        pair<ll, int> p = q.top();
        q.pop();
        int v = p.second;
        if (dis[v] < p.first) continue;
        for (edge e : G[v]) {
            if (dis[e.to] > dis[v] + e.cost) {
                dis[e.to] = dis[v] + e.cost;
                q.push({dis[e.to], e.to});
            }
        }
    }
}

/*
����ͼ����NΪ�����һ�� dijkstra����¼N������������·��
�ٶ���ÿһ�εĸɲݶѲ�������N+1��pos֮�佨һ������ΪN�������ľ����x�ıߡ�
����N+1Ϊ�����һ��dijkstra�󣬴�1��Nö�٣�
���N��i�����·С��N+1��i�����·, ��ô�����1���������0��
*/
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    cin>>n>>m>>k;
    while (m--) {
        int u, v;
        ll w;
        cin>>u>>v>>w;
        G[u].push_back({v, w});
        G[v].push_back({u, w});
    }
    dijkstra(n);
    for (int i = 1; i <= n; i++) {
        ans[i] = dis[i];
    }
    
    for (int i = 1; i <= k; i++) {
        int pos;
        ll x;
        cin>>pos>>x;
        G[n + 1].push_back({pos, dis[pos] - x});
    }
    dijkstra(n + 1);
    for (int i = 1; i <= n; i++) {
        if (dis[i] <= ans[i]) cout << 1 << endl;
        else cout << 0 << endl;
    }

    return 0;
}