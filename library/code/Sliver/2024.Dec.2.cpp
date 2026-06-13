#include <bits/stdc++.h>
using namespace std;

int TC, N, K, x[100001];
int dis[100001], visited[100001];
vector<pair<int, int> > E[100001];

void dijkstra() {
    priority_queue<pair<int, int> > Q;  // 最大堆
    for (int i = 0; i <= N; i++) {
        dis[i] = 1e9;
        visited[i] = 0;
    }
    dis[0] = 0;
    Q.push({0, 0});

    while (!Q.empty()) {
        int u = Q.top().second;
        Q.pop();
        if (visited[u]) continue;
        visited[u] = 1;
        for (auto &e : E[u])
            if (dis[e.first] > dis[u] + e.second) {
                dis[e.first] = dis[u] + e.second;
                Q.push({-dis[e.first], e.first});  // -dis[e.first]: priority 负数最大值对应正数为最小值
            }
    }
}

int main() {
	cin >> TC;
    while (TC--) {
		cin >> N >> K;
        for (int i = 1; i <= N; i++) cin >> x[i];
        sort(x + 1, x + N + 1);
        for (int i = 0; i <= N; i++) E[i].clear();

        // shortest path
        // dis[i]: number of trees cut among [1, i]

        for (int i = 1; i <= K; i++) {
            int l, r, w;
			cin >> l >> r >> w;
            l = lower_bound(x + 1, x + N + 1, l) - x - 1;
            r = upper_bound(x + 1, x + N + 1, r) - x - 1;
            w = r - l - w;
            // at most w trees can be cut in (l, r]
            // dis[r] - dis[l] <= w
            E[l].push_back({r, w});
        }

        for (int i = 0; i < N; i++) {
            // dis[i + 1] - dis[i] <= 1
            // dis[i + 1] - dis[i] >= 0 => dis[i] - dis[i + 1] <= 0
            E[i].push_back({i + 1, 1});
            E[i + 1].push_back({i, 0});
        }

        dijkstra();
		cout << dis[N] << '\n';
    }
    return 0;
}
