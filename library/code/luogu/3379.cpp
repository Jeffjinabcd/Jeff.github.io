#include <bits/stdc++.h>
using namespace std;

const int N = 500001;

int n,m,s;
int depth[N], Log[N];
int f[N][20]; //倍增数组 

vector<int> g[N];

void dfs(int cur, int fa) {
    depth[cur]=depth[fa]+1;
    f[cur][0]=fa;
    for (int i = 1; (1 << i) < depth[cur]; ++i) {
        int mid = f[cur][i - 1];
        f[cur][i] = f[mid][i - 1];
    }
    for (auto to : g[cur]) {
        if (to != fa) {
            dfs(to, cur);
        }
    }
}

int lca(int x, int y) {
	// 把两个点升至同一高度，再一起跳
    // TODO
    if (depth[x] < depth[y]) {
        swap(x, y);
    }
    while (depth[x] > depth[y]) {
        x = f[x][Log[depth[x] - depth[y]]];
    }

    if (x == y) return x;
    // 两个点同时往上跳，跳到LCA的下一层为止
    // TODO
    for (int i = Log[depth[x]];i >= 0; --i) {
        if (f[x][i] != f[y][i]) {
            x = f[x][i];
            y = f[y][i];
        }
    }

    return f[x][0];
}

/*
倍增算法时间复杂度是O(nlogn)
*/
int main() {
    scanf("%d%d%d",&n,&m,&s);
    for(int i=1; i<=n-1; i++) {
        int x,y;
        scanf("%d%d",&x,&y);
        g[x].push_back(y);
        g[y].push_back(x);
    }
    dfs(s,0); // 建树

    // 预处理，常数优化
    for(int i=2; i<=n; i++) {
        Log[i]=Log[i>>1] + 1;
    }

    for(int i=1; i<=m; i++) {
        int x, y;
        scanf("%d%d", &x, &y);
        printf("%d\n", lca(x, y));
    }
    return 0;
}

