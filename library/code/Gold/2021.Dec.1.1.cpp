#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 5;
int T, n, k, f[N][2], flag = 1, las;

struct cow {
    int x, y;
} a[N];

/*
考虑如果选择了一些奶牛不匹配，首先需要满足这些不匹配的奶牛之间的距离需要大于
k，然后还要满足剩下奶牛可以匹配。

为了使剩下的奶牛尽量可以匹配，显然相邻两个匹配最优。

设f[i][j]表示前i头奶牛中，未匹配奶牛数是奇数头还是偶数头
（j=0表示偶数，j=1表示奇数）的最大或最小值。

通过i,j的奇偶性，就可以判断出i前面匹配了的奶牛数量的奇偶性。

直接分情况转移，用las表示满足las到i的距离大于k的最大的las。

前面选择了偶数头奶牛，那么必然可以两两匹配，
如果第i头奶牛未匹配，那么就是f[las][1-j]+y[i]
如果第i头奶牛匹配，这头奶牛需要与之前的进行匹配，因此需要满足i-1到i距离小于等于k，
如果满足，就转移f[i-1][j]。

前面选择了奇数头奶牛，那么必然剩下一头奶牛没有匹配，
如果第i头奶牛未匹配，需要让前一头奶牛与后面一头奶牛匹配，因此需要满足i-1到i+1距离小于等于k，
那么转移f[las][1-j]+y[i]
如果第i头奶牛匹配，这头奶牛需要与后续的进行匹配，因此需要满足i到i+1距离小于等于k，
如果满足，就转移f[i-1][j]
*/
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    cin >> T >> n >> k;
    if (T == 2) flag = -1; // 取正数和的最大值，可以转化为求负数和的最小值，方便统一处理
    for (int i = 1; i <= n; i++) {
        cin >> a[i].x >> a[i].y;
        a[i].y *= flag;
    }
    a[0].x = -1e9, a[n + 1].x = 1e9;
    memset(f, 0x3f, sizeof(f));
    f[0][0] = 0;
    for (int i = 1; i <= n; i++) {
        // 满足las到i的距离大于k的最大的位置
        while (a[i].x - a[las + 1].x > k) las++;
        int j = i & 1; // j表示i的奇偶性, 因此f[i][j]表示前面选择了偶数头奶牛
        // 前面选择了偶数头奶牛匹配
        // TODO
        f[i][j] = min(f[i][j], f[las][j ^ 1] + a[i].y);
        if (a[i].x - a[i - 1].x <= k) {
            f[i][j] = min(f[i][j], f[i - 1][j]);
        }
        // 前面选择了奇数头奶牛匹配
        // TODO
        if (a[i + 1].x - a[i - 1].x <= k) {
            f[i][j ^ 1] = min (f[i][j ^ 1], f[las][j] + a[i].y);
        }
        if (a[i + 1].x - a[i].x <= k) {
            f[i][j ^1] = min (f[i][j^1], f[i - 1][j ^ 1]);
        }
    }

    cout << f[n][n & 1] * flag;
    return 0;
}