#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int MAXN = 101;
const int MOD = 9999973;
int N, M;
ll f[MAXN][MAXN][MAXN];
// f[i][j][k]表示放了前i行，有j列是有1个棋子，有k列有2个棋子的方案数

inline int C2(int num) {  // 相当于C2(num,2)
    return num * (num - 1) / 2;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    // n行m列的棋盘
    cin >> N >> M;
    f[0][0][0] = 1;
    for (int i = 1; i <= N; i++) {
        for (int j = 0; j <= M; j++) {
            for (int k = 0; k <= M - j; k++) {
                // 不放
                f[i][j][k] = (f[i][j][k] + f[i - 1][j][k]) % MOD;
                // TODO 放一个，在有一个棋子的那一列
                if (k >= 1) {
                    f[i][j][k] = (f[i][j][k] + f[i - 1][j+1][k-1] * (j+1)) % MOD;
                }

                // TODO 放一个，在没有棋子的那一列
                if (j >= 1) {
                    f[i][j][k] += f[i - 1][j - 1][k] * (M - j - k + 1) % MOD;
                }

                // TODO 放两个，一个在没有棋子的列，一个在有一个棋子的列
                if (k >= 1) {
                    f[i][j][k] = (f[i][j][k] + f[i - 1][j][k-1] * (M - j - k + 1) * j) % MOD;
                }
                // TODO 放两个，都在没有棋子的两列
                if (j >= 2) {
                    f[i][j][k] += f[i - 1][j - 2][k] * C2((M - j - k + 2)) % MOD;
                }
                // TODO 两个，在一个棋子的两列
                if (k >= 2) {
                    f[i][j][k] = (f[i][j][k] + f[i - 1][j+2][k-2] * C2(j+2)) % MOD;
                }

                f[i][j][k] %= MOD;
            }
        }
    }

    ll ans = 0;
    for (int i = 0; i <= M; ++i)            // 有1个棋子的列
        for (int j = 0; i + j <= M; ++j) {  // 2个棋子的列
            ans = (ans + f[N][i][j]) % MOD;
        }
    cout << ans << endl;

    return 0;
}