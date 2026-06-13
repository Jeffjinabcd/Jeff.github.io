#include <bits/stdc++.h>
#define ll long long
using namespace std;
const int maxn = 100000 + 10;
ll A[maxn << 1], Sum[maxn << 3], Add[maxn << 3], Mul[maxn << 3];
ll n, s, p;

void PushUp(ll rt) {
    Sum[rt] = (Sum[rt << 1] + Sum[rt << 1 | 1]) % p;
}  //上推函数

void PushDown(ll rt, ll m)  //下推函数，注意乘法优先级比加法高
{
    if (Add[rt] || Mul[rt] != 1) {
        Sum[rt << 1] = (Sum[rt << 1] * Mul[rt] + Add[rt] * (m - (m >> 1))) % p;
        Sum[rt << 1 | 1] = (Sum[rt << 1 | 1] * Mul[rt] + Add[rt] * (m >> 1)) % p;
            
        Add[rt << 1] = (Add[rt << 1] * Mul[rt] + Add[rt]) % p;
        Add[rt << 1 | 1] = (Add[rt << 1 | 1] * Mul[rt] + Add[rt]) % p;
        
        Mul[rt << 1] = (Mul[rt << 1] * Mul[rt]) % p;
        Mul[rt << 1 | 1] = (Mul[rt << 1 | 1] * Mul[rt]) % p;
        
        Add[rt] = 0;
        Mul[rt] = 1;
    }
}

void Build(ll l, ll r, ll rt)  //建树
{
    Add[rt] = 0;
    Mul[rt] = 1;
    if (l == r) {
        Sum[rt] = A[l] % p;
        return;
    }
    ll m = (l + r) >> 1;
    Build(l, m, rt << 1);
    Build(m + 1, r, rt << 1 | 1);
    PushUp(rt);
}

void Update_mul(ll L, ll R, ll C, ll l, ll r, ll rt)  //区间乘法运算
{
    PushDown(rt, (r - l + 1));
    if (L <= l && r <= R) {
        Sum[rt] = Sum[rt] * C;
        Sum[rt] = Sum[rt] % p;
        Mul[rt] = Mul[rt] * C;
        Mul[rt] = Mul[rt] % p;
        return;
    }
    ll m = (l + r) >> 1;
    if (L <= m) Update_mul(L, R, C, l, m, rt << 1);
    if (R > m) Update_mul(L, R, C, m + 1, r, rt << 1 | 1);
    PushUp(rt);
}

void Update_plu(ll L, ll R, ll C, ll l, ll r, ll rt)  //区间加法运算
{
    PushDown(rt, r - l + 1);
    if (L <= l && r <= R) {
        Sum[rt] = (Sum[rt] + (r - l + 1) * C) % p;
        Add[rt] = (Add[rt] + C) % p;
        return;
    }
    ll m = (l + r) >> 1;
    if (L <= m) Update_plu(L, R, C, l, m, rt << 1);
    if (R > m) Update_plu(L, R, C, m + 1, r, rt << 1 | 1);
    PushUp(rt);
}

ll Query(ll L, ll R, ll l, ll r, ll rt)  //区间查询
{
    PushDown(rt, r - l + 1);
    if (L <= l && r <= R) {
        return Sum[rt];
    }
    ll m = (l + r) >> 1, Ans = 0;
    if (L <= m) Ans = (Ans + Query(L, R, l, m, rt << 1)) % p;
    if (R > m) Ans = (Ans + Query(L, R, m + 1, r, rt << 1 | 1)) % p;
    return Ans;
}

/*
区间和的形式应该为：ax+b
在程序中我们把mtp定义为a，sum定义为x，inc定义为b，下传标记时节点的更新就是sum = mtp * sum + inc

当我们要修改一个区间时，要保证ax+b的形式，即先乘后加的形式。
当将区间乘以一个数k时，原来的区间和为ax+b，乘以k得k(ax+b)=kax+kb，也就是把节点的inc和mtp都乘上一个k。
区间加一个数更加简单，原来的区间和为ax+b，加上一个k为ax+b+k, 合并b, k得ax+(b+k)，也就是把原来的inc加上一个k。
*/
int main()  //各种操作
{
    scanf("%lld%lld", &n, &p);
    for (int i = 1; i <= n; i++) scanf("%lld", &A[i]);
    Build(1, n, 1);
    scanf("%lld", &s);
    for (int i = 1; i <= s; i++) {
        ll t, x, y, k;
        scanf("%lld%lld%lld", &t, &x, &y);
        if (t == 1) {
            scanf("%lld", &k);
            Update_mul(x, y, k, 1, n, 1);
        }
        if (t == 2) {
            scanf("%lld", &k);
            Update_plu(x, y, k, 1, n, 1);
        }
        if (t == 3) {
            printf("%lld\n", Query(x, y, 1, n, 1));
        }
    }
    return 0;
}
