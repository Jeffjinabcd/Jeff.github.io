#include <bits/stdc++.h>
using namespace std;

struct person {
    int t, x;  // 到达时间，国籍
};

int nat[300005];  // 每个国籍人数
int cnt;          // 总的国籍数

queue<person> q;  // 本题用queue或deque均可

/*
滑动窗口问题
*/
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin >> n;
    for (int i = 1; i <= n; i++) {
        int t, k, x;
        cin >> t >> k;
        // 达到的船只
        for (int j = 1; j <= k; j++) {
            cin >> x;
            if (nat[x] == 0) cnt++;  // 新增国籍
            nat[x]++;                // 该国籍人数增加
            q.push({t, x});
        }
        // 离开24小时区间
        while (t - 86400 >= q.front().t) {
            nat[q.front().x]--;                // 该国籍人数减少
            if (nat[q.front().x] == 0) cnt--;  // 这个国籍没人了，总数减少
            q.pop();
        }
        cout << cnt << endl;
    }

    return 0;
}
