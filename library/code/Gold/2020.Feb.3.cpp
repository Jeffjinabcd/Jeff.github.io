#include <bits/stdc++.h>
using namespace std;
const int MAXN = 1e5 + 5;

int n, sub[MAXN]; // sub[i]代表以i为根节点的子树共有多少个节点
vector<int> g[MAXN], num[MAXN];  // num[i].size()代表i有多少子树，其中每个数代表每颗子树的节点数

void dfs(int cur, int fa) {
    sub[cur] = 1;
    for (int son : g[cur]) {
        if(son == fa) continue;

        dfs(son, cur);
        sub[cur] += sub[son];
        num[cur].push_back(sub[son]);
    }

    if (sub[cur] != n) num[cur].push_back(n - sub[cur]); // 非根节点
}

bool check(int K) {
    // 如果不能均分，返回false
    if ((n - 1) % K != 0) return false;

    int cur[MAXN] = {};  // 零头计数
    for (int i = 1; i <= n; ++i) {
        int cnt = 0; // 零头条数
        for (int t : num[i]) {
            int z = t % K; // t等于子树中的边数+1
            if (z == 0) continue;
            if (cur[K - z])  //如果可以配对，就配
                cur[K - z]--, cnt--;
            else             //如果不能配对，就留
                cur[z]++, cnt++;
        }
        if (cnt) return false;  // paths don't pair up
    }
    return true;
}

int main() {
    freopen("deleg.in", "r", stdin);
    freopen("deleg.out", "w", stdout);
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);

    cin >> n;
    for (int i = 1; i < n; ++i) {
        int a, b;
        cin >> a >> b;
        g[a].push_back(b);
        g[b].push_back(a);
    }
    dfs(1, 0);
    for (int i = 1; i < n; ++i) {
        if (check(i))
            cout << 1;
        else
            cout << 0;
    }
    cout << endl;
    return 0;
}
