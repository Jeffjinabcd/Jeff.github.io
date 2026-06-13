#include <bits/stdc++.h>
using namespace std;
#define ll long long

int n, k;
int a[10010];
int f[10010];

int main () {
    freopen("teamwork.in", "r", stdin);
    freopen("teamwork.out", "w", stdout);
    cin >> n >> k;
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
    }
    for (int i = 1; i <= n; ++i) {
        // int m = a[i];
        // for (int j = 1; j <= k && i - j + 1 >= 1; ++j) {
        //     m = max (m, a[i - j + 1]);
        //     f[i] = max(f[i], f[i - j] + j * m);
        // }
        int m = a[i];
        for (int j = i; j >= i - k + 1 && j >= 1; --j) {
            m = max (m, a[j]);
            f[i] = max(f[i], f[j - 1] + (i - j + 1) * m);
        }
    }
    cout << f[n];
}