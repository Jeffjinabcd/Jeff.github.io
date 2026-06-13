#include <bits/stdc++.h>
using namespace std;

int n, cnt, L, W, x, r;

struct line {
    double x, y;
};

line a[20005];

bool cmp(const line &x, const line &y) { 
    return x.x < y.x;
}

void solve() {
    sort(a + 1, a + cnt + 1, cmp);  // 按照左端点从小到大排序

    double last = 0;  // 当前已选区间的最后一个点
    int ans = 0;
    int i = 1;
    while (last < L) {
        double s = last;
        // 依次找能够覆盖s点的区间中右端点坐标最大的一个
        for (; i <= cnt; i++) {
            if (a[i].x <= s) {
                last = max(last, a[i].y);
            } else {
                break;
            }
        }

        if (last == s)  // 如果中间有断档
        {
            cout << -1 << endl;
            return;
        }

        ans++;
    }
    cout << ans << endl;
}

int main() {
    int T;
    cin >> T;
    for (int i = 0; i < T; i++) {
        cin >> n >> L >> W;
        cnt = 0;
        for (int i = 1; i <= n; i++) {
            cin >> x >> r;
            if (2 * r <= W) continue;
            cnt++;
            double len = sqrt(r * r - W * W / 4.0);
            a[cnt].x = x - len;
            a[cnt].y = x + len;
        }
        solve();
    }
    return 0;
}
