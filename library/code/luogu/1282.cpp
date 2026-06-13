#include <bits/stdc++.h>
using namespace std;

int n, a[1010], b[1010];
int f[1010][10100];

int main () {
    cin >> n;
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
        cin >> b[i];
    }
    memset(f. 0x7f, sizeof(f));
    f[0][5050] = 0;
    for (int i = 1; i <= n; ++i) {
        for (int j = -5000; j <= 5000; ++j) {
            int delta = a[i] - b[i];
            f[i][j + 5050] = min(f[i - 1][j + 5050 - delta], f[i - 1][j + 5050 + delta] + 1);
        }
    }
    int index = 0;
    while (f[n][index] == 0x7f) {
        ++index;
    }
    cout << f[n][index];
}
