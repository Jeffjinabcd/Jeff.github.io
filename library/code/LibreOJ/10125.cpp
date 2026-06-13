#include <bits/stdc++.h>
using namespace std;
#define LL long long

const int SIZE = 1e6;

struct node{
	int l, r;
	LL w; // 区间和 
};

node tree[4*SIZE + 1]; // 开4倍大小

inline void build(int k, int l, int r) { // k代表当前节点编号
	tree[k].l = l;
	tree[k].r = r;
    if (l == r) { // 叶子节点 
        scanf("%lld", &tree[k].w);
        return;
    }
    int mid = (l + r) >> 1;
    int lc = k << 1, rc = lc | 1;
    build(lc, l, mid); // 左子树
    build(rc, mid + 1, r); // 右子树
    tree[k].w = tree[lc].w + tree[rc].w;
}

// 单点修改
inline void changePoint(int k, int x, int c) {
    if (tree[k].l == tree[k].r) { // 找到叶子节点
        tree[k].w += c;
        return;
    }
    int mid = (tree[k].l + tree[k].r) >> 1;
    int lc = k << 1, rc = lc | 1;
    if (x <= mid)
        changePoint(lc, x, c);
    else
        changePoint(rc, x, c);
    tree[k].w = tree[lc].w + tree[rc].w;
}

// 区间查询
inline LL queryInterval(int k, int L, int R) {
    if (tree[k].l >= L && tree[k].r <= R)
        return tree[k].w;

    int mid = (tree[k].l + tree[k].r) >> 1;
    int lc = k<<1, rc = lc | 1;
    LL ans = 0;
    if (L <= mid)
        ans += queryInterval(lc, L, R);
    if (R > mid)
        ans += queryInterval(rc, L, R);
    return ans;
}

int main() {
    int n, q;
	scanf("%d%d", &n, &q);
    build(1, 1, n);
    for (int i = 1; i <= q; i++) {
        int opt, x, y;
        scanf("%d%d%d", &opt, &x, &y);
        if (opt == 1)
            changePoint(1, x, y);
        if (opt == 2)
            printf("%lld\n", queryInterval(1, x, y));
    }
    return 0;
}
