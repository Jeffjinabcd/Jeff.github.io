#include <bits/stdc++.h>
using namespace std;

const int N = 5e4 + 5;

struct Edge {
	int to, w;
};

vector<Edge> g[N];
int dis[N];
bool inq[N];

int spfa(int S, int T) {
	queue<int> q;
	q.push(S);
	memset(dis, 0xcf, sizeof(dis)); // 初始化为最小值
	dis[S] = 0;
	inq[S] = true;
	
	while (!q.empty()) {
		int u = q.front();
		q.pop();
		inq[u] = false;
		
		for (auto e : g[u]) {
			int to = e.to, w = e.w;
			
			// 求单源最长路
			if (dis[to] < dis[u] + w) {
				dis[to] = dis[u] + w;

				if (!inq[to]) {
					q.push(to);
					inq[to] = true;
				}					
			}
		}
	}
	
	return dis[T];
}

int main() {
	int n;
	scanf("%d", &n);
	
	// s[k + 1]-s[k] <= 1 变形为 s[k]-s[k+1] >= -1
	for (int i = 0; i <= 50000; i ++)
		g[i + 1].push_back({i, -1});
	
	// s[k+1]-s[k] >= 0
	for (int i = 0; i <= 50000; i ++)
		g[i].push_back({i + 1, 0});
	
	for (int i = 1; i <= n; i ++) {
		int l, r, c;
		scanf("%d%d%d", &l, &r, &c);
		// s[r+1] - s[l] >= c
		g[l].push_back({r + 1, c});
	}
	
	printf("%d\n", spfa(0, 50001));
	
	return 0;
}
