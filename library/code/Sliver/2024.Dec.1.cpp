#include <bits/stdc++.h>
using namespace std;

long long T, N, a[500010], prefix_sum[500010], E, B;



int main () {
    cin >> T;
    for (int q = 0; q < T; ++q) {
        E = 0;
        B = 0;
        cin >> N;
        for (int i = 1; i <= N; ++i) {
            cin >> a[i];
            prefix_sum[i] = prefix_sum[i - 1] + a[i];
        }
        long long count = N / 2 - 1;
        for (int i = 0; i <= count; ++i) {
            E = max(prefix_sum[i] + prefix_sum[N] - prefix_sum[N - (count - i)], E);
        }
        B = prefix_sum[N] - E;
        cout << B << " " << E << endl;
    }
}