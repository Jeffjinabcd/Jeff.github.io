#include <bits/stdc++.h>
using namespace std;
const int N = 100005;
int c[30][30], f[N];
char s[N];
int sum[30][N];
int query(int x, int l, int r)
{
    return sum[x][r] - sum[x][l - 1];
}
int minn[30];

/*
给定一个字符串和一个M×M 的矩阵a，表示可以消耗Aij的代价把字符i改成字符j，
求把原字符串改成每个连续段长度都不小于K的最小代价。
*/
int main()
{
    freopen("cowmbat.in", "r", stdin);
    freopen("cowmbat.out", "w", stdout);
    int n, m, k; // m个按键
    scanf("%d%d%d", &n, &m, &k);
    scanf("%s", s + 1);
    for (int i = 0; i < m; i++)
        for (int j = 0; j < m; j++)
            scanf("%d", &c[i][j]);
    // Floyd, 因为直接用原矩阵的交换方法不一定最优
    for (int x = 0; x < m; x++)
        for (int i = 0; i < m; i++)
            for (int j = 0; j < m; j++) {
                c[i][j] = min (c[i][j], c[i][x] + c[x][j]);
            }

    for (int i = 0; i < m; i++)
        for (int j = 1; j <= n; j++) {
            sum[i][j] = sum[i][j - 1] + c[s[j] - 'a'][i];
        }

    // f[i]表示前i个中每个连续段都≥k的最小代价
    // f[i]=min(f[i], f[j-1] + cost(j, i, c))   j<=i-k
    // cost(j, i, c) 表示把[j, i]全都改成字符c的最小代价，可以使用前缀和预处理
    memset(minn, 0x3f, sizeof(minn));
    memset(f, 0x3f, sizeof(f));
    f[0] = 0;
    for (int i = k; i <= n; i++)
        for (int j = 0; j < m; j++) {
            minn[j] = min (minn[j] + c[s[i] - 'a'][j], f[i - k] + query(j, i - k + 1, i));
            f[i] = min (f[i], minn[j]);
        }
    printf("%d", f[n]);
    return 0;
}