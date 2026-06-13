#include <bits/stdc++.h>
using namespace std;

typedef long long LL;

const int SIZE = 2e5 + 1;

struct segtree{
	int l, r;
	int w;
};

segtree tree[4*SIZE]; // 开4倍大小 

void build(int k, int l, int r) { // k代表当前节点编号 
	tree[k].l = l;
	tree[k].r = r;
    if (l == r) {
        tree[k].w=0;
        return;
    }
    int mid = l + r >> 1;
    int lc = k<<1, rc = lc + 1;
    build(lc, l, mid); // 左子树
    build(rc, mid + 1, r); // 右子树
//    tree[k].w = max(tree[lc].w, tree[rc].w);
}

void changePoint(int k, int x, int c) {
    if (tree[k].l == tree[k].r) {
        tree[k].w += c;
        return;
    }
    int mid = (tree[k].l + tree[k].r) >> 1;
    int lc = k<<1, rc = lc + 1;
    if (x <= mid)
        changePoint(lc, x, c);
    else
        changePoint(rc, x, c);
    tree[k].w = max(tree[lc].w, tree[rc].w);
}

// 区间查询
int queryInterval(int k, int L, int R) {
    if (tree[k].l >= L && tree[k].r <= R)
        return tree[k].w;

    int mid = tree[k].l + tree[k].r >> 1;
    int lc = k<<1, rc = lc + 1;
    int ans = 0;
    if (L <= mid)
        ans = max(ans, queryInterval(lc, L, R));
    if (R > mid)
        ans = max(ans, queryInterval(rc, L, R));
    return ans;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0), cout.tie(0);
    
    int m, p, n=0, a=0;
	cin>>m>>p;
	build(1, 1, m);
    for (int i = 1; i <= m; i++) {
        char opt;
		int value;
        cin>>opt>>value;
        if (opt == 'A') {
        	n++;
			changePoint(1, n, (value+a)%p);
		}

        if (opt == 'Q') {
        	a = queryInterval(1, n-value+1, n);
        	cout<<a<<endl;
		}
    }
    return 0;
}
