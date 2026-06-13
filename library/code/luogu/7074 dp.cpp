#include <bits/stdc++.h>
using namespace std;

int n, m;
int a[1001][1001];
long long up[1005], down[1005], f[1005];

/**
每一列，要么只能往上走，要么只能往下走。
为什么呢？不能重复经过已经走过的方格。

up[i][j]表示从左侧或下面走上来到i行j列时的最大值和。
down[i][j]表示从左侧或上面走下来到i行j列时的最大值。
f[i][j]表示i行j列最终可取到的最大值。

状态转移方程
up[i][j]=max(up[i+1][j],f[i][j-1])+a[i][j];
down[i][j]=max(down[i-1][j],f[i][j-1])+a[i][j];
f[i][j]=max(up[i][j],down[i][j]);

最大值仅与上一列的最大值有关，故可降一维：
up[i]=max(up[i+1], f[i])+a[i][j];
down[i]=max(down[i-1], f[i])+a[i][j];
f[i]=max(up[i], down[i]);

时间复杂度 O(nm)。
*/
int main() {
    // freopen("name.in","r",stdin);
    // freopen("name.out","w",stdout);
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);

    cin >> n >> m; // n rows m cols
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++) {
            cin >> a[i][j];
        } 

    // 第一列只可能往下走
    f[1] = a[1][1];
    for (int i = 2; i <= n; i++) {
        f[i] = f[i - 1] + a[i][1];
    }
    for (int C = 2; C < m; C++) {
        memset(down, 0, sizeof(down));
        memset(up, 0, sizeof(up));
        down[1] = f[1] + a[1][C];
        up[n] = f[n] + a[n][C];
        for (int R = 2; R <= n; R++) {
            down[R] = max(f[R], down[R - 1]) + a[R][C];
        }
        for (int R = n - 1; R >= 1; R--) {
            up[R] = max(f[R], up[R + 1]) + a[R][C];
        }
        for (int R = 1; R <= n; R++) {
            f[R] = max(down[R], up[R]);
        }
    }

    // 最后一列只可能往下走
    f[1] += a[1][m];
    for (int i = 2; i <= n; i++) {
        f[i] = max(f[i], f[i - 1]) + a[i][m];
    }
    cout << f[n] << endl;

    return 0;
}