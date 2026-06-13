#include <bits/stdc++.h>
using namespace std;

int n, m;
int C[30], V[30];
int f[30010];

int main () {
    cin >> n >> m;
    for (int i = 1; i <= m; ++i) {
        cin >> C[i] >> V[i];
        V[i] *= C[i];
    }
    for (int i = 1; i <= m; ++i) {
        for (int v = n; v >= C[i]; --v) {
            f[v] = max(f[v], f[v - C[i]] + V[i]);
        }
    }
    cout << f[n];
}