#include <bits/stdc++.h>
using namespace std;

struct Point {
    int t, id;  // 时间点  牛的Id
};
Point s[200001];

set<int> onduty;  // 存目前有几个开始点，但是对应的结束点还没出现

int alone[100001];
int totalCover;

bool cmp(const Point &a, const Point &b) {
    return a.t < b.t;
}

int main() {
    freopen("lifeguards.in", "r", stdin);
    freopen("lifeguards.out", "w", stdout);

    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);

    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        int st, en;
        cin >> st >> en;
        s[i] = {st, i};
        s[i + n] = {en, i};
    }
    sort(s, s + 2 * n, cmp);
    set<int>::iterator it;  // 记录当前区间被哪些牛看守
    for (int i = 0; i < 2 * n; i++) {
        // 某头牛单独覆盖该区间
        if (onduty.size() == 1) {
            it = onduty.begin();
            alone[*it] += s[i].t - s[i - 1].t;
        }
        // 有牛覆盖该区间
        if (!onduty.empty()) {
            totalCover += s[i].t - s[i - 1].t;
        }
        if (onduty.count(s[i].id) > 0)
            onduty.erase(s[i].id);
        else
            onduty.insert(s[i].id);
    }

    int minAlone = INT_MAX;
    for (int i = 0; i < n; i++) {
        minAlone = min(minAlone, alone[i]);
    }
    cout << totalCover - minAlone << endl;

    return 0;
}
