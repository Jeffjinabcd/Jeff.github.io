#include <bits/stdc++.h>
using namespace std;

int N;
int a[7505];
int b[7505];
int pre[7505];
int f[7505][7505];
int res[7505];

int main () {
    cin >> N;
    for (int i = 1; i <= N; ++i) {
        cin >> a[i];
    }
    for (int i = 1; i <= N; ++i) {
        cin >> b[i];
    }
    for (int i = 1; i <= N; ++i) {
        pre[i] += pre[i - 1];
        if (a[i] == b[i]) {
            pre[i] += 1;
        }
    }
    for (int i = 1; i <= N; ++i) {
        if (a[i] == b[i]) {
            f[i][i] = 1;
        }
        if (i != N) {
            f[i][i + 1] = (a[i] == b[i + 1]) + (a[i + 1] == b[i]);
        }
    }
    for (int i = N; i > 0; --i) {
        for (int j = i + 2; j <= N; ++j) {
            f[i][j] = f[i + 1][j - 1] + (a[i] == b[j]) + (a[j] == b[i]);
        }
    }
    for (int i = 1; i <= N; ++i) {
        for (int j = i; j <= N; ++j) {
            ++res[f[i][j] + pre[i - 1] + pre[N] - pre[j]];
        }
    }
    for (int i = 0; i <= N; ++i) {
        cout << res[i] << endl;
    }
}