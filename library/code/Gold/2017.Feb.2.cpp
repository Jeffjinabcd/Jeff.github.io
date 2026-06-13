#include <bits/stdc++.h>
using namespace std;

int N;
int A[1010], B[1010], f[1010][1010];
int main () {
    freopen("nocross.in", "r", stdin);
    freopen("nocross.out", "w", stdout);
    cin >> N;
    int temp;
    for (int i = 1; i <= N; ++i) {
        cin >> temp;
        A[i] = temp;
    }
    for (int i = 1; i <= N; ++i) {
        cin >> temp;
        B[i] = temp;
    }
    for (int i = 1; i <= N; ++i) {
        for (int j = 1; j <= N; ++j) {
            if (abs(A[i] - B[j]) <= 4) {
                f[i][j] = f[i - 1][j - 1] + 1;
            }
            else {
                f[i][j] = max(f[i - 1][j], f[i][j - 1]);
            }
        }
    }
    cout << f[N][N];
}