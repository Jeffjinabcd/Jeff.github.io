#include <bits/stdc++.h>
#define LL long long
using namespace std;

struct interval{
    int l, r, t;
} q[100000];

int T, N, K, cnt, a[300010], x[100000], c[300010];

void discrete (int n, int k) {
    sort(a + 1, a + cnt + 1);
    cnt = unique(a + 1, a + cnt + 1) - a - 1;

    for (int i = 1; i <= n; ++i) {
        x[i] = lower_bound(a + 1, a + cnt + 1, x[i]) - a;
    }
    sort(x + 1, x + n + 1);
    for (int i = 1; i <= k; ++i) {
        q[i].l = lower_bound(a + 1, a + cnt + 1, q[i].l - 1) - a;
        q[i].r = lower_bound(a + 1, a + cnt + 1, q[i].r) - a;
    }
}

bool cmp (const interval &a, const interval &b){
    return a.r < b.r;
}

int lowbit(int x) {
    return x & (-x);
}

void update(int x, int v) {
    for (int i = x; i <= cnt; i += lowbit(i)) c[i] += v;
}

int query(int l) {
    int sum = 0;
    for (int i = l; i >= 1; i -= lowbit(i)) sum += c[i];
    return sum;
}

int main () {
    cin >> T;
    for (int p = 0; p < T; ++p) {
        cin >> N >> K;
        for (int i = 1; i <= N; ++i) {
            cin >> x[i];
            a[i] = x[i];
        }
        cnt = N;
        for (int i = 1; i <= K; ++i) {
            cin >> q[i].l >> q[i].r >> q[i].t;
            a[++cnt] = q[i].l - 1;
            a[++cnt] = q[i].r;
        }
        discrete(N, K);
        memset(c, 0, sizeof(c));
        priority_queue<int> pq;
        sort(q + 1, q + K + 1, cmp);
        int now = 0, ans = N;
        for (int i = 1; i <= K; ++i) {
            int num = query(q[i].l);
            while (now < N && x[now + 1] <= q[i].r) {
                pq.push(x[++now]);
            }
            while (query(q[i].r) - num < q[i].t) {
                ans--;
                update(pq.top(), 1);
                pq.pop();
            }
        }
        cout << ans << endl;
    }
}