#include <bits/stdc++.h>
using namespace std;

const int N = 1005;

const int DIR[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
const char DIR_SIGN[4] = {'U', 'D', 'L', 'R'}; // ע�⣬����ķ������µ�λ��ָ��ǰλ�õķ���

char a[N][N];
bool vis[N][N];
int n, x[200005], y[200005];

queue<pair<int, int>> q;
int cnt;

void bfs() {
	while (!q.empty()) {
		int x1 = q.front().first, y1 = q.front().second;
		q.pop();
		for (int i = 0; i < 4; i++) {
			int x2 = x1 + DIR[i][0], y2 = y1 + DIR[i][1];
			if (!vis[x2][y2] && x2 <= n && x2 >= 1 && y2 <= n && y2 >= 1 && 
				(a[x2][y2] == DIR_SIGN[i] || !a[x2][y2])) {
				vis[x2][y2] = 1;
				q.push({x2, y2});
				--cnt;
			}
		}
	}
}

int ans[200005];
int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	
	int Q;
	cin >> n >> Q;
	cnt = n * n;
	for (int i = 1; i <= Q; i++) {
		cin >> x[i] >> y[i] >> a[x[i]][y[i]];
	}
	
	for (int i = 1; i <= n; i++) {
		if (!vis[1][i] && (a[1][i] == 'U' || !a[1][i])) {
			q.push({1, i});
			vis[1][i] = 1;
			--cnt;
		}
		if (!vis[i][1] && (a[i][1] == 'L' || !a[i][1])) {
			q.push({i, 1});
			vis[i][1] = 1;
			--cnt;
		}
		if (!vis[n][i] && (a[n][i] == 'D' || !a[n][i])) {
			q.push({n, i});
			vis[n][i] = 1;
			--cnt;
		}
		if (!vis[i][n] && (a[i][n] == 'R' || !a[i][n])) {
			q.push({i, n});
			vis[i][n] = 1;
			--cnt;
		}
	}
	
	bfs();
	
	ans[Q] = cnt;
	for (int i = Q; i > 1; i--) {
		a[x[i]][y[i]] = 0;
		if (!vis[x[i]][y[i]]) {
			if (x[i] == 1 || x[i] == n || y[i] == 1 || y[i] == n) {
				vis[x[i]][y[i]] = 1;
				q.push({x[i], y[i]});
				--cnt;
				bfs();
			}
			if (!vis[x[i]][y[i]] && (
				vis[x[i] + DIR[0][0]][y[i] + DIR[0][1]] || 
				vis[x[i] + DIR[1][0]][y[i] + DIR[1][1]] || 
				vis[x[i] + DIR[2][0]][y[i] + DIR[2][1]] || 
				vis[x[i] + DIR[3][0]][y[i] + DIR[3][1]])) {
					vis[x[i]][y[i]] = 1;
					q.push({x[i], y[i]});
					--cnt;
					bfs();
			}
		}
		ans[i - 1] = cnt;
	}
	for (int i = 1; i <= Q; i++) {
		cout << ans[i] << '\n';
	}
	return 0;
}
