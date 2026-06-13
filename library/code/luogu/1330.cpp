#include <bits/stdc++.h>
using namespace std;

vector<int> g[10005];

// vis: 是否遍历过
// color: 每一个点的染色
int vis[10005], color[10005];
int ans;

bool bfs(int begin) {
    queue<int> q;
    q.push(begin);
    vis[begin] = true;
    int cnt_0 = 1, cnt_1 = 0;
    while (!q.empty()){
        int current = q.front();
        q.pop();
        int len = g[current].size();
        for (int i = 0; i < len; ++i){
            int next = g[current][i];
            if (vis[next]){
                if (color[current] != color[next]){
                    continue;
                }
                else {
                    return false;
                }
            }
            else {
                vis[next] = true;
                q.push(next);
                if (color[current] == 0){
                    color[next] = 1;
                    ++cnt_1;
                }
                else {
                    color[next] = 0;
                    ++cnt_0;
                }
            }
        }
    }
    ans += min(cnt_1, cnt_0);
    return true;
}

/*
①每一条边所连接的点中，至少要有一个被选中。
②每一条边所连接的两个点，不能被同时选中。
由此，可以推断出：每一条边都有且仅有一个被它所连接的点被选中。
又因为我们要处理的是一个连通图。所以，对于这一个图的点的选法，
可以考虑到相邻的点染成不同的颜色。

于是，对于一个连通图，要不就只有两种选法（因为可以全部选染成一种色的，
也可以全部选染成另一种色的），要不就是impossible！所以，我们只需要找
到每一个子连通图，对它进行黑白染色，然后取两种染色中的最小值，然后最
后汇总，就可以了。另外，要判断impossible，只需要加一个used数组，记录
已经遍历了哪些点。如果重复遍历一个点，且与上一次的颜色不同，则必然是
impossible的。
*/
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, m;
    cin >> n >> m;
    for (int i = 1; i <= m; i++) {
        int a, b;
        cin >> a >> b;
        g[a].push_back(b); // 无向图，要双向加边
        g[b].push_back(a);
    }

    for (int i = 1; i <= n; i++) {  // 可能不是一个连通图，需要循环
        if (vis[i]) {
            continue;
        }

        if (!bfs(i)) {
            cout << "Impossible" << endl;
            return 0;
        }
    }

    cout << ans << endl;

    return 0;
}
