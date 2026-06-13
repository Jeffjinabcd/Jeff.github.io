#include <bits/stdc++.h>
using namespace std;

bool f[5000010];
int T, A, B;
int main () {
    cin >> T >> A >> B;
    f[0] = 1;
    for (int i = A; i <= T; ++i) {
        if (f[i - A]) {
            f[i] = 1;
        }
    }
    for (int i = B; i <= T; ++i) {
        if (f[i - B]) {
            f[i] = 1;
        }
    }
    for (int i = 0; i * 2 + 1 <= T; ++i) {
        if (i * 2 <= T && f[i * 2]) {
            f[i + A] = 1;
            f[i + B] = 1;
        }
        else if (i * 2 + 1 <= T && f[i * 2 + 1]) {
            f[i + A] = 1;
            f[i + B] = 1;
        }
    }
    for (int i = A; i <= T; ++i) {
        if (f[i - A]) {
            f[i] = 1;
        }
    }
    for (int i = B; i <= T; ++i) {
        if (f[i - B]) {
            f[i] = 1;
        }
    }
    for (int i = T; i >= 0; --i) {
        if (f[i]) {
            cout << i;
            return 0;
        }
    }
    cout << 0;
}