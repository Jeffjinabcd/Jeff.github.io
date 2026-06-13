#include <bits/stdc++.h>
using namespace std;

const int N = 1e5 + 10;
int n, m, t[N], c[N], ans[N], top[N];
vector<int> graph[N], query[N];

// 遍历到节点k ，只需记录[1,k]上，每种Ci的最深点的儿子
void dfs(int cur, int fa) {
    int backup = top[t[cur]];
    for (int v : graph[cur]) {
        if (v == fa) continue;
        top[t[cur]] = v;  // 存儿子编号
        dfs(v, cur);
    }

    // 遍历到点cur时，将关于cur的所有询问遍历一遍（先离线询问）
    for (int x : query[cur]) {
        // 若询问第一次遍历到，标记询问，即[1,i]上深度最深、点权为p的节点，
        if (ans[x] == -1)
            ans[x] = top[c[x]];
        else  //遍历到点j时，比较标记的点与
              //[1,j]上深度最深、点权为p的节点是否相同
            ans[x] = (top[c[x]] != ans[x]);
    }

    top[t[cur]] = backup;
}

/*
Tarjan求LCA的入门题
动态维护每种品种的牛奶的最深点的子节点，询问时查找两个节点的最深点的儿子是否相同

题意：
给你一棵树，每个点有点权，每次询问节点 i 到节点 j 的路径上是否有点权为 p 的点。
*/
int main() {
    // freopen("milkvisits.in", "r", stdin);
    // freopen("milkvisits.out", "w", stdout);
    ios::sync_with_stdio(false);
    cin.tie(0);
    memset(ans, -1, sizeof(ans));
    cin >> n >> m;
    for (int i = 1; i <= n; i++) cin >> t[i];
    int x, y, z;
    for (int i = 1; i < n; i++) {
        cin >> x >> y;
        graph[x].push_back(y);
        graph[y].push_back(x);
    }
    for (int i = 1; i <= m; i++) {
        cin >> x >> y;
        cin >> c[i];
        if (t[x] == c[i] || t[y] == c[i]) ans[i] = 1;
        query[x].push_back(i);
        query[y].push_back(i);
    }
    dfs(1, 0);
    for (int i = 1; i <= m; i++) cout << ans[i];

    return 0;
}