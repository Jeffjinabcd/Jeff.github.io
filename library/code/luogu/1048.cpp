#include <bits/stdc++.h>
using namespace std;

int T, M;
int C[110], V[110], f[110][1010], ff[1010];

int main () {
    cin >> T >> M;
    for (int i = 1; i <= M; ++i) {
        cin >> C[i] >> V[i];
    }
    // for (int i = 1; i <= M; ++i) {
    //     for (int v = T; v > 0; --v) {
    //         if (v - C[i] >= 0){
    //             f[i][v] = max(f[i - 1][v], f[i - 1][v - C[i]] + V[i]);
    //         }
    //         else {
    //             f[i][v] = f[i - 1][v];
    //         }
    //     }
    // }
    for (int i = 1; i <= M; ++i) {
        for (int v = T; v >= C[i]; --v) {
            ff[v] = max(ff[v], ff[v - C[i]] + V[i]);
        }
    }
    cout << ff[T];
}