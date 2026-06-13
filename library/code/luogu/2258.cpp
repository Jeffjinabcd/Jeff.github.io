#include<bits/stdc++.h>
using namespace std;

int n,m,r,c,ans=INT_MAX;
int book[20], a[20][20], sumx[20];
int f[20][20];  // f[i][j]表示前i列选j列且选了第i列的最小分数

void check() {
    memset(f,0x3f,sizeof(f));
    memset(sumx, 0, sizeof(sumx));
    for(int i=1; i<=m; i++) // column
        for(int j=1; j<r; j++) {
			// 计算每一列上r行的相邻差的绝对值之和
			sumx[i]+=abs(a[book[j]][i]-a[book[j+1]][i]); 
		}

    for(int i=1;i<=m;i++)
        f[i][1]=sumx[i];  // 边界条件 

    for (int i = 1; i <= m; ++i) {
        for (int j = 2; j <= c; ++j) {
            for (int k = j - 1; k < i; ++k) {
                int sumy = 0;
                for (int x = 1; x <= r; ++x) {
                    sumy += abs(a[book[x]][i] - a[book[x]][k]);
                }
                f[i][j] = min (f[i][j], f[k][j - 1] + sumx[i] + sumy);
            }
        }
    }
    for (int i = c; i <= m; ++i) {
        ans = min (ans, f[i][c]);
    }
}

void dfs(int x,int pre) { // x:子矩阵的第x行， pre: 子矩阵的上一行在原矩阵中对应的行数 
    if (x > r) {
        check();
        return;
    }
    for (int i = pre + 1; i <= n; ++i) {
        book[x] = i;
        dfs(x + 1, i);
    }
}

int main() {
//	freopen("submatrix.in", "r", stdin);
//    freopen("submatrix.out", "w", stdout);
    scanf("%d%d%d%d",&n,&m,&r,&c);
    for(int i=1; i<=n; i++)
        for(int j=1; j<=m; j++)
            scanf("%d",&a[i][j]);
            
    dfs(1,0);
    
    printf("%d\n",ans);
    return 0;
}

