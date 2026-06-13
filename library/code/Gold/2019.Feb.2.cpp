#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 1;
int n, maxCleanId, base[N];
vector<int> plates[N];
/*
每次拿一个盘子x打好肥皂，执行以下处理：
1，找到一堆打好肥皂的盘子，满足该堆盘子里至少有一个盘子编号大于x, 且该盘子越左越好。
2，如果该堆盘子中有小于x的，就把所有小于x的盘子拿走洗掉以保证单调性
*/
int main() {
    freopen("dishes.in", "r", stdin);
    freopen("dishes.out", "w", stdout);
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    cin >> n;
    for (int i = 1; i <= n; i++) {
        int x;
        cin >> x;
        if (x < maxCleanId) {
            cout << i - 1;
            return 0;
        }
        for (int j = x; j > 0 && !base[j]; --j) {
            base[j] = x;
        }
        while (!plates[base[x]].empty() && plates[base[x]].back() < x) {
            maxCleanId = plates[base[x]].back();
            plates[base[x]].pop_back();
        }
        plates[base[x]].push_back(x);
    }
    cout << n << endl;
    return 0;
}