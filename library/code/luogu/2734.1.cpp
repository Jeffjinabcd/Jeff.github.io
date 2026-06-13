#include <bits/stdc++.h>
using namespace std;

int N;
int num[101];
int f[101][101];
int s[101];

int main () {
    cin >> N;
    for (int i = 1; i <= N; ++i) {
        cin >> num[i];
        f[i][i] = num[i];
        s[i] = s[i - 1] + num[i];
    }
    
    for (int i = N; i > 0; --i) {
        for (int j = i + 1; j <= N; ++j) {
            f[i][j] = max(s[j] - s[i - 1] - f[i + 1][j], s[j] - s[i - 1] - f[i][j - 1]);
        }
    }
    cout << f[1][N] << " " << s[N] - f[1][N];
}