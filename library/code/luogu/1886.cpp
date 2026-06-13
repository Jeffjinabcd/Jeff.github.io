#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1e6 + 5;

int n, k;

struct node {
    int id, x;
};

int a[MAXN];
int winMin[MAXN], winMax[MAXN];  // 每次窗口滑动的最小/大值

deque<node> qmin, qmax;

void q_min(node e) {
    // 从队尾删除
    while (qmin.size() > 0 && qmin.back().x >= e.x) qmin.pop_back();

    // 从队首删除
    if (qmin.size() > 0 && qmin.front().id + k == e.id) qmin.pop_front();

    qmin.push_back(e);

    if (e.id >= k) winMin[e.id - k + 1] = qmin.front().x;
}

void q_max(node e) {
    // 从队尾删除
    while (qmax.size() > 0 && qmax.back().x <= e.x) qmax.pop_back();

    // 从队首删除
    if (qmax.size() > 0 && qmax.front().id + k == e.id) qmax.pop_front();

    qmax.push_back(e);

    if (e.id >= k) winMax[e.id - k + 1] = qmax.front().x;
}

int main() {
    cin >> n >> k;
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
        q_min({i, a[i]});
        q_max({i, a[i]});
    }

    for (int i = 1; i <= n - k + 1; i++) {
		cout << winMin[i] << " ";
	}
    cout << endl;
    for (int i = 1; i <= n - k + 1; i++) {
		cout << winMax[i] << " ";
	}

    return 0;
}
