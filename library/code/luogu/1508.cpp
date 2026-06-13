
#include <bits/stdc++.h>
using namespace std;

int a[201][201], f[201][201];
int main() {
    int n, m, x, y;
    cin >> n >> m;
    x = n;
    y = m / 2 + 1;  //最开始的位置
    memset(f, 0xff, sizeof(f));
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            cin >> a[i][j];
        }
    }
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            f[i][j] = max(max(f[i - 1][j], f[i - 1][j - 1]), f[i - 1][j + 1]) + a[i][j];
        }
    }
    cout << max(max(f[x][y], f[x][y - 1]), f[x][y + 1]) << endl;
    return 0;
}
