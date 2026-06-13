#include <bits/stdc++.h>
#define LL long long
using namespace std;

const LL MAXN = 100000 + 10;
int T, n;
LL f[MAXN];

struct node {
    LL a, b, c;
};

node m[MAXN];

bool cmp(const node &x, const node &y) {  // c[x]*b[y]<c[y]*b[x]
    return x.c * y.b < y.c * x.b;
}

/*
01背包衍生
如果没有b[i]这个属性的话就是明显的01背包问题。
现在考虑相邻的两个物品x,y。假设现在已经耗费p的时间，那么分别列出先做x,先做y的美味指数：
①a[x]-(p+c[x])*b[x]  +  a[y]-(p+c[x]+c[y])*b[y]
②a[y]-(p+c[y])*b[y]  +  a[x]-(p+c[y]+c[x])*b[x]
对这两个式子化简，得到①＞②的条件是 c[x]*b[y]<c[y]*b[x].
只要满足这个条件的物品对(x,y)，x在y前的美味指数永远更优。
因此可以根据这个条件进行排序，之后就是简单的01背包了。
*/
int main() {
    cin >> T >> n;
    for (int i = 1; i <= n; i++) cin >> m[i].a;
    for (int i = 1; i <= n; i++) cin >> m[i].b;
    for (int i = 1; i <= n; i++) cin >> m[i].c;
    sort(m + 1, m + 1 + n, cmp);

    for (int i = 1; i <= n; i++)
        for (int j = T; j >= m[i].c; j--)
            f[j] = max(f[j], f[j - m[i].c] + m[i].a - j * m[i].b);

    LL maxx = 0;
    for (int i = 1; i <= T; i++)  // 由于美味指数可能为负数，因此，必须枚举最大值
        maxx = max(f[i], maxx);
    cout << maxx;
    return 0;
}
