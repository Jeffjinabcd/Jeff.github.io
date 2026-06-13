#include <bits/stdc++.h>
using namespace std;

const int N = 2501;
const int M = 15001;

struct edge {
    int to, w;
};

int t, c, ts, te;
int rs, re, ci;
int dis[N];
bool vis[N];

vector<edge> g[N];

struct point {
    int id;
    int len;
};

struct cmp  //�º���
{
    bool operator()(const point &a, const point &b) {
        return a.len > b.len;
    }
};

priority_queue<point, vector<point>, cmp> pq;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    cin >> t >> c >> ts >> te;
    for (int i = 0; i < c; i++) {
        cin >> rs >> re >> ci;
        g[rs].push_back({re, ci});
        g[re].push_back({rs, ci});
    }

    memset(dis, 0x3f, sizeof(dis));
    dis[ts] = 0;  // ע���� ʼ����ts������
    pq.push({ts, 0});

    while (!pq.empty()) {
        int id = pq.top().id;
        pq.pop();

        if (vis[id]) {
            continue;
        }
        vis[id] = true;

        for (auto &k :  g[id]) {
            if (dis[id] + k.w < dis[k.to]) {
                dis[k.to] = dis[id] + k.w;
                pq.push({k.to, dis[k.to]});
            }
        }
    }

    cout << dis[te] << endl;
    return 0;
}
