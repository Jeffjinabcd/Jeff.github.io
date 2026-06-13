#include <bits/stdc++.h>
using namespace std;

int N, ans;
struct item {
    int q, t, x, n;
};

item p[200001];

map<int, int> mp;

bool cmp(const item &a, const item &b) {
	if (a.x - a.t != b.x - b.t) {
        return a.x - a.t < b.x - b.t;
	}
    return a.q > b.q; // 相等时苹果事件在前
}

/*
假如在苹果出现的同一时刻，一头奶牛出现在了和苹果重合的位置，奶牛就可以将苹果采走。
显然，事件i中出现的奶牛能采到事件j中出现的苹果，当且仅当ti<=tj且abs(xi-xj)<=tj-ti.
把不等式转化一下：xj-tj <= xi-ti且xi+ti <= xj+tj
所以，我们可以把每个事件i都表示成一组二维坐标(xi-ti, xi+ti)
问题就转化成了：在二维平面上有n个点，每个点处有若干头奶牛或若干个苹果，如果?个苹果
位于?头奶牛的左上方，就可以建立匹配，每个苹果或奶牛最多属于一个匹配，问最大匹配是多少。
*/
int main() {
	ios::sync_with_stdio(false);
	cin.tie(0), cout.tie(0);
    cin >> N;
    for (int i = 1; i <= N; i++) {
        cin >> p[i].q >> p[i].t >> p[i].x >> p[i].n;
	}
    sort(p + 1, p + N + 1, cmp);

    for (int i = 1; i <= N; i++) {
        if (p[i].q == 2) // 当遇到苹果事件时：将ni个苹果放到平衡树中的位置xi+ti处。
            mp[p[i].x + p[i].t] += p[i].n;
        else {
			// 贪心策略：奶牛选取苹果时，取的都是能取的苹果中最不容易被别的奶牛取的苹果，
			// 这样一定能保证最优。
			// 当遇到奶牛事件时：奶牛优先选取在平衡树上位置>=xi+ti且位置尽量近的苹果，直到该事件中
            // 的奶牛全部取到苹果，或所有位置>=xi+ti的苹果都被取完。
            int n = p[i].n;
            while (n) {
                auto it = mp.lower_bound(p[i].x + p[i].t);
                if (it == mp.end()) break;
                int cnt = min(n, it-> second);
                if (cnt == it-> second) {
                    mp.erase(it);
                }
                else {
                    it->second -= cnt;
                }
                n -= cnt;
                ans += cnt;
            }
        }
    }

    cout << ans << endl;
    return 0;
}
