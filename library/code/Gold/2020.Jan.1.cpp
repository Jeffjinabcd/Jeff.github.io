#include <bits/stdc++.h>
#define LL long long
using namespace std;

const int MAXN = 1005;
const int MAXT = 1005;

LL n, m, c;
LL value[MAXN];
LL dp[2][MAXN];
// dp[t][x] the maximum amount of moonies Bessie can make up to time t 
// if she is located at city x at time t

vector<pair<int, int> > edges;

/*
 * O(max(mi).(N+M))
 * The final answer will be max(dp[1][t]-C*t^2), 0¡Üt¡Ü1000 
 * 1000*t-t^2,  which is negative when t>1000, so t<=1000
 */
int main() {
    // freopen("time.in", "r", stdin);
    // freopen("time.out", "w", stdout);
    ios::sync_with_stdio(false);
    cin.tie(0);
    // city money constant
    cin >> n >> m >> c;
	for (int i = 1; i <= n; i++) {
		cin >> value[i];
	}
	int a, b;
	for (int i = 0; i < m; i++) {
		cin >> a >> b;
		edges.push_back({a, b});
	}
	LL ans = 0;
	memset(dp, -1, sizeof(dp));
	dp[0][1] = 0;
	for (int t = 1; t < MAXT; t++) {
		int p = t % 2;
		memset(dp[p], -1, sizeof(dp[p]));
		// for (int i = 0; i<edges.size(); i++) {
		for (auto e : edges) {
			a = e.first;
			b = e.second;
			if (dp[1-p][a] >= 0) {
				dp[p][b] = max(dp[p][b], dp[1-p][a] + value[b]);
			}
		}
		ans = max(ans, dp[p][1] - c * t * t);
	}
	cout << ans << endl;
    return 0;
}