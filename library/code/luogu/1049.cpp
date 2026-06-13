#include <bits/stdc++.h>
using namespace std;

int V, n;
int obj[20010];
int f[20010];

int main() {
    cin >> V >> n;
    for (int i = 1; i <= n; ++i) {
        cin >> obj[i];
    }
    for (int i = 1; i <= n; ++i) {
        for (int v = V; v >= obj[i]; --v) {
            f[v] = max(f[v], f[v - obj[i]] + obj[i]);
            if (f[v] == V) {
                cout << 0;
                return 0;
            }
        }
    }
    cout << V - f[V];
}