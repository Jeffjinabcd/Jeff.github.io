#include <bits/stdc++.h>
using namespace std;

int x[22][22];

int main () {
    int n, m;
    cin >> n >> m;
    for (int i = 1; i < 23; ++i) {
        x[i][1] = 1;
        x[1][i] = 1;
    }

    for (int i = 2; i < 23; ++i) {
        for (int j = 2; j < 23; ++j) {
            x[i][j] = x[i - 1][j] + x[i][j - 1];
        }
    }
    cout << x[n][m];
}