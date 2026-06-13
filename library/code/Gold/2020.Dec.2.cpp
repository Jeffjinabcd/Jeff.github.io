#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1e9 + 7;
const int MAXN = 100002;

char s[MAXN];
int n;
// dp[i][a][b][c]表示最后一段最后一个字母为a,
// 首字母为b，倒数第二段首字母为c的基因组序列数量。
int dp[MAXN][4][4][4];
string to = "AGCT";
int ans = 0;

// inline int mod(int x) { return x < MOD ? x : x - MOD; }
// inline void add(int &x, int y) { x = mod(x + y); }

/*
合法串有两个条件:
1、每一段划分中间不能有连续相同字符，不然我们一定会在那个位置将其割开。
2、对于划分中的连续两个子串s,t，s的首字符必须等于t的末字符，不然我们也不会去分割那个位置。
*/
int main() {
    scanf("%s", s + 1);
    n = strlen(s + 1);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (s[1] == '?' || s[1] == to[j]) dp[1][j][j][i] = 1;
        }
    }

    for (int i = 2; i <= n; i++) {
        for (int a = 0; a < 4; ++a) {
            if (s[i] != '?' && s[i] != to[a]) continue;
            for (int b = 0; b < 4; b++) {
                for (int c = 0; c < 4; ++c) {
                    for (int la = 0; la < 4; ++la) {
                        if (la != a) {
                            dp[i][a][b][c] = (dp[i][a][b][c] + dp[i - 1][la][b][c]) % MOD;
                        }
                        if (la == c) {
                            dp[i][a][a][b] = (dp[i][a][a][b] + dp[i - 1][la][b][c]) % MOD;
                        }
                    }
                }
            }
        }
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ans = (ans + dp[n][i][j][i]) % MOD;
        }
    }

    printf("%d", ans);
    return 0;
}