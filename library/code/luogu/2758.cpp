#include <bits/stdc++.h>
using namespace std;

string a, b;
int f[2010][2010];

int main () {
    cin >> a >> b;
    for (int i = 1; i <= a.length(); ++i) {
        f[i][0] = i;
    }
    for (int j = 1; j <= b.length(); ++j) {
        f[0][j] = j;
    }
    for (int i = 1; i <= a.length(); ++i) {
        for (int j = 1; j <= b.length(); ++j) {
            if (a[i - 1] == b[j - 1]) {
                f[i][j] = f[i - 1][j - 1];
            }
            else {
                f[i][j] = min (f[i - 1][j - 1], min (f[i][j - 1], f[i - 1][j])) + 1;
            }
        }
    }
    cout << f[a.length()][b.length()];
}