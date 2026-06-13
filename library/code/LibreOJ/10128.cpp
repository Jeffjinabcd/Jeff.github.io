#include <bits/stdc++.h>
using namespace std;

typedef long long LL;

const int SIZE = 1e5;

struct node{
	int l, r;
	LL w;
	int tag;
};

node tree[4*SIZE]; // 开4倍大小 

inline void pushUp(int k) {
	int lc = k<<1;
	tree[k].w=tree[lc].w+tree[lc|1].w;
	tree[k].tag=tree[lc].tag && tree[lc|1].tag;
}

inline void build(int k, int l, int r) { // k代表当前节点编号 
	tree[k].l = l;
	tree[k].r = r;
    if (l == r) {
        scanf("%lld", &tree[k].w);
        if(tree[k].w<=1) tree[k].tag=1;
        return;
    }
    int mid = l + r >> 1;
    int lc = k<<1, rc = lc | 1;
    build(lc, l, mid); // 左子树 
    build(rc, mid + 1, r); // 右子树 
    pushUp(k);
}

// 标记下传 
inline void pushDown(int k)
{
     if (tree[k].tag) {
        return;
     }
     if (tree[k].l == tree[k].r) {
        tree[k].w = sqrt(tree[k].w);
        if (tree[k].w <= 1) {
            tree[k].tag = 1;
        }
        return;
     }
     int lc = k << 1, rc = lc | 1;
     pushDown(lc);
     pushDown(rc);
     pushUp(k);
}

// 区间查询 
inline LL queryInterval(int k, int L, int R) {
    if (tree[k].l >= L && tree[k].r <= R)
        return tree[k].w;

    int mid = tree[k].l + tree[k].r >> 1;
    int lc = k<<1, rc = lc | 1;
    LL ans = 0;
    if (L <= mid)
        ans = queryInterval(lc, L, R);
    if (R > mid)
        ans += queryInterval(rc, L, R);
    return ans;
}

//区间修改
inline void updateInterval(int k, int L, int R)
{
	if(tree[k].tag)
		return;
	
	if(tree[k].l>=L&&tree[k].r<=R)
	{
		pushDown(k);
		return;
	}
	
	int mid = tree[k].l + tree[k].r >> 1;
    int lc = k<<1, rc = lc | 1;
	if(L<=mid)
		updateInterval(lc, L, R);
	if(R>mid)
		updateInterval(rc, L, R);
	pushUp(k);
}

/*
一个数多开几次都会变成1，故用tag标记，tag=1时表示不用开方了。
*/ 
int main() {
    int n;
    scanf("%d", &n);
    build(1, 1, n);
    int m;
    scanf("%d", &m);
    for (int i = 1; i <= m; i++) {
        int opt, x, y;
        scanf("%d%d%d", &opt, &x, &y);
        if (opt == 1)
        	printf("%lld\n", queryInterval(1, x, y));

        if (opt == 2)
            updateInterval(1, x, y);
    }
    return 0;
}
