#include <bits/stdc++.h>
using namespace std;

int n, m;
int currency[100];
int f[100];

int main () {
    cin >> n >> m;
    for (int i = 1; i <= n; ++i) {
        cin >> currency[i];
    }
    f[0] = 1;
    for (int i = 1; i <= n; ++i) {
        for (int v = currency[i]; v <= m; ++ v) {
            f[v] = f[v] + f[v - currency[i]];
        }
    }
    cout << f[m];
}