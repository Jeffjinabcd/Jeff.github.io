#include <bits/stdc++.h>
using namespace std;

int T, M;
long long C[10010], V[10010], ff[10000010];

int main () {
    cin >> T >> M;
    for (int i = 1; i <= M; ++i) {
        cin >> C[i] >> V[i];
    }
    for (int i = 1; i <= M; ++i) {
        for (int v = C[i]; v <= T; ++v) {
            ff[v] = max(ff[v], ff[v - C[i]] + V[i]);
        }
    }
    cout << ff[T];
}