#include <bits/stdc++.h>
#define LL long long
using namespace std;

const int MAXN = 401;
LL a[MAXN], b[MAXN][MAXN],
    dp[MAXN][MAXN];  // dp[i][j]表示前i组蛇，用了j个网的最小代价
int N, M;

int main() {
    // freopen("name.in","r",stdin);
    // freopen("name.out","w",stdout);
    ios::sync_with_stdio(false);
    cin.tie(0);

    cin >> N >> M;
    M++;  // 刚开始算一个
    for (int i = 1; i <= N; i++) cin >> a[i];

    // 预处理, b[i][j]表示用一个网把区间[i,j]的蛇全抓走的代价
    for (int i = 1; i <= N; i++) {
        LL sum = 0, maxn = 0;
        for (int j = i; j <= N; j++) {
            maxn = max(maxn, a[j]);
            sum += a[j];
            b[i][j] = maxn * (j - i + 1) - sum;
        }
    }

    memset(dp, 0x3f, sizeof(dp));
    dp[0][0] = 0;
    for (int j = 1; j <= M; j++)
        for (int i = 1; i <= N; i++)
            for (int k = 0; k < i; k++) {
                dp[i][j] = min(dp[i][j], dp[k][j - 1] + b[k + 1][i]);
            }

    cout << dp[N][M] << endl;
    return 0;
}