#include <bits/stdc++.h>
using namespace std;

int n, m, k;
char A[1010], B[210];
int f[2][210][210][2];

int main () {
    cin >> n >> m >> k;
    for (int i = 1; i <= n; ++i) {
        cin >> A[i];
    }
    for (int i = 1; i <= m; ++i) {
        cin >> B[i];
    }
    f[0][0][0][0] = 1;
    f[1][0][0][0] = 1;
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            for (int o = 1; o <= k; ++o) {
                f[i%2][j][o][0] = f[(i - 1)%2][j][o][0] + f[(i - 1)%2][j][o][1];
                if (A[i] == B[j]) {
                    f[i%2][j][o][1] = f[(i - 1)%2][j - 1][o][1] + f[(i - 1)%2][j - 1][o - 1][0] + f[(i - 1)%2][j - 1][o - 1][1];
                }
                else {
                    f[i%2][j][o][1] = 0;
                }
                f[i%2][j][o][0] = f[i%2][j][o][0] % 1000000007;
                f[i%2][j][o][1] = f[i%2][j][o][1] % 1000000007;
            }
        }
    }
    cout << (f[n%2][m][k][0] + f[n%2][m][k][1]) % 1000000007;
    return 0;
}