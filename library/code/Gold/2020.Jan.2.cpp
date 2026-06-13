#include <bits/stdc++.h>
using namespace std;

#define LL long long

LL n, q;
const LL offset = 1e6;
LL a[5005];
LL l[100010][2];
LL f[5005][5005];
LL num[2*offset + 1];

int main () {
    freopen("threesum.in", "r", stdin);
    freopen("threesum.out", "w", stdout);
    cin >> n >> q;
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
    }
    for (int i = 1; i <= q; ++i) {
        cin >> l[i][0] >> l[i][1];
    }
    for (int i = 1; i <= n; ++i) {
        for (int j = i + 2; j <= n; ++j) {
            ++num[a[j - 1] + offset];
            LL ak = - a[i] - a[j];
            if (ak <= offset && ak >= -offset) {
                f[i][j] = num[ak + offset];
                // cout << i << " " << j << ": " << c[i][j] << endl;
            }
        }
        for (int j = 1; j <= n; ++j) {
            num[a[j] + offset] = 0;
        }
    }
    for (int i = n; i > 0; --i) {
        for (int j = i + 1; j <= n; ++j) {
            f[i][j] += f[i + 1][j] + f[i][j - 1] - f[i + 1][j - 1];
        }
    }
    for (int i = 1; i <= q; ++i) {
        cout << f[l[i][0]][l[i][1]] << endl;
    }
}