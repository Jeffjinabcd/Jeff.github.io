#include <bits/stdc++.h>
#define ll long long
using namespace std;

const int MAXN = 2501;

int x[MAXN], y[MAXN];
int t[MAXN];
int p[MAXN][MAXN];
int prefix[MAXN][MAXN];
map<int, int> mx, my;

int rangesum(int x1, int y1, int x2, int y2) {  // 左上角  右下角
    return prefix[x2][y2] - prefix[x1 - 1][y2] - prefix[x2][y1 - 1] +
           prefix[x1 - 1][y1 - 1];
}

int arr[MAXN];
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    int n;
    cin >> n;
    for (int i = 1; i <= n; i++) cin >> x[i] >> y[i];

    for (int i = 1; i <= n; i++) {
        arr[i] = x[i];
    }
    sort(arr + 1, arr + n + 1);
    for (int i = 1; i <= n; i++) mx[arr[i]] = i;
    for (int i = 1; i <= n; i++) x[i] = mx[x[i]];  // 离散化

    for (int i = 1; i <= n; i++) {
        arr[i] = y[i];
    }
    sort(arr + 1, arr + n + 1);
    for (int i = 1; i <= n; i++) my[arr[i]] = i;
    for (int i = 1; i <= n; i++) y[i] = my[y[i]];  // 离散化

    for (int i = 1; i <= n; i++) {
        t[x[i]] = y[i];
        p[x[i]][y[i]] = 1;
    }

    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++) {
            // 二维前缀和
            prefix[i][j] = prefix[i - 1][j] + prefix[i][j - 1] -
                           prefix[i - 1][j - 1] + p[i][j];
        }

    ll ans = 1 + n;  // 空集 + 单点集

    for (int i = 1; i <= n; i++) {        // 最上端点
        for (int j = i + 1; j <= n; j++)  // 最下端点
        {
            int yi = t[i], yj = t[j];
            if (yi > yj) {
                swap(yi, yj);
            }
            ans += (rangesum(i, 1, j, yi - 1) + 1) * (rangesum(i, yj + 1, j, n) + 1);
        }
    }
    cout << ans << endl;

    return 0;
}