#include <bits/stdc++.h>
using namespace std;
int pasture[510][510], colmin[510];
int main() {
    int n;
    cin >> n;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++) {
            scanf("%d", &pasture[i][j]);
        }

    long long ans = 0;
    for (int i = 1; i <= n; i++)
        for (int j = i; j <= n; j++) {
            int left = 0, cnt = 0;
            for (int r = 1; r <= n; r++) {
                if (i == j) {
                    colmin[r] = pasture[j][r];
                } else {
                    colmin[r] = min(colmin[r], pasture[j][r]);
                }

                if (colmin[r] < 100) {
                    left = r;
                    cnt = 0;
                }
                if (colmin[r] == 100) {
                    cnt = r - left;
                }
                ans += cnt;
            }
        }

    cout << ans << endl;
    return 0;
}