#include <bits/stdc++.h>
using namespace std;

typedef long long LL;

int n, m, c;
int a[100010], d[100010];
vector<pair<int, int>> g[100010];
queue<int> q;

int main () {
    freopen("timeline.in", "r", stdin);
    freopen("timeline.out", "w", stdout);
    cin >> n >> m >> c;
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
    }
    int u, v, w;
    for (int i = 1; i <= c; ++i) {
        cin >> u >> v >> w;
        g[u].push_back({v, w});
        d[v]++;
    }
    for (int i = 1; i <= n; ++i) {
        if (d[i] == 0) {
            q.push(i);
        }
    }
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        for (auto &e : g[cur]) {
            a[e.first] = max(a[e.first], a[cur] + e.second);
            d[e.first]--;
            if (d[e.first] == 0) {
                q.push(e.first);
            }
        }
    }
    for (int i = 1; i <= n; ++i) {
        cout << a[i] << endl;
    }
}