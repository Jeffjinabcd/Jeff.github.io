#include <bits/stdc++.h>
#define LL long long
using namespace std;

const int SIZE = 1e6;

struct node {
    int l, r;
    LL w;
    LL tag;
};

node tree[4 * SIZE];  // 开4倍大小

inline void build(int k, int l, int r) {  // k代表当前节点编号
    tree[k].l = l;
    tree[k].r = r;
    if (l == r) {
        scanf("%lld", &tree[k].w);
        return;
    }
    int mid = l + r >> 1;
    int lc = k << 1, rc = lc | 1;
    build(lc, l, mid);      // 左子树
    build(rc, mid + 1, r);  // 右子树
    tree[k].w = tree[lc].w + tree[rc].w;
}

// 标记下传
inline void pushDown(int k) {
    if (tree[k].tag == 0) return;
    int lc = k << 1, rc = lc | 1;
    // 当前节点的懒标记累积到子节点的懒标记中
    tree[lc].tag += tree[k].tag;
    tree[rc].tag += tree[k].tag;
    // 修改子节点状态
    tree[lc].w += tree[k].tag * (tree[lc].r - tree[lc].l + 1);
    tree[rc].w += tree[k].tag * (tree[rc].r - tree[rc].l + 1);
    // 父节点懒标记清0
    tree[k].tag = 0;
}

// 区间查询
inline LL queryInterval(int k, int L, int R) {
    if (tree[k].l >= L && tree[k].r <= R) {
        return tree[k].w;
    }

    pushDown(k);

    int mid = tree[k].l + tree[k].r >> 1;
    int lc = k << 1, rc = lc | 1;
    LL ans = 0;
    if (L <= mid) ans += queryInterval(lc, L, R);
    if (R > mid) ans += queryInterval(rc, L, R);
    return ans;
}

//区间修改
inline void updateInterval(int k, int L, int R, int y) {
    if (tree[k].l >= L && tree[k].r <= R) {
        tree[k].w += 1LL * y * (tree[k].r - tree[k].l + 1);
        tree[k].tag += y;
        return;
    }
    pushDown(k);

    int mid = tree[k].l + tree[k].r >> 1;
    int lc = k << 1, rc = lc | 1;
    if (L <= mid) updateInterval(lc, L, R, y);
    if (R > mid) updateInterval(rc, L, R, y);
    tree[k].w = tree[lc].w + tree[rc].w;
}

int main() {
    int n, q;
    scanf("%d%d", &n, &q);
    build(1, 1, n);
    for (int i = 1; i <= q; i++) {
        int opt, x, y;
        scanf("%d%d%d", &opt, &x, &y);
        if (opt == 1) {
            int val;
            scanf("%d", &val);
            updateInterval(1, x, y, val);
        }
        if (opt == 2) printf("%lld\n", queryInterval(1, x, y));
    }
    return 0;
}
