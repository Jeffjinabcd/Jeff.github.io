#include <bits/stdc++.h>
using namespace std;

int M, N;
int obj[10010], f[10010];

int main () {
    cin >> N >> M;
    for (int i = 1; i <= N; ++i) {
        cin >> obj[i];
    }
    f[0] = 1;
    for (int i = 1; i <= N; ++i) {
        for (int v = M; v >= obj[i]; --v) {
            f[v] += f[v - obj[i]];
        }
    }
    cout << f[M];
}
