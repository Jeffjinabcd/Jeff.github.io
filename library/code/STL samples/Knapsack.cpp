#include <bits/stdc++.h>
using namespace std;

struct p{
    long long v, w;
} c[3000010];

long long S, N, v, w, k, n1, f[20001];


int main () {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> S >> N;
    for (int i = 1; i <= N; ++i) {
        cin >> v >> w >> k;
        int t = 1;
        while (k >= t){
            c[++n1].v = v*t;
            c[n1].w = w*t;
            k -= t;
            t *= 2;
        }
        if (k > 0) {
            c[++n1].v = v*k;
            c[n1].w = w*k;
        }
    }
    for (int i = 1; i <= n1; ++i) {
        for (int j = S; j >= c[i].w; --j) {
            f[j] = max(f[j], f[j - c[i].w] + c[i].v);
        }
    }
    cout << f[S];
}