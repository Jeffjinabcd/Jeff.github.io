#include <bits/stdc++.h>
using namespace std;

const int MAXN = 400005;
// f为并查集，h为打击点存储的数组，ans为每次打击后的答案
int f[MAXN], h[MAXN], ans[MAXN];
bool vis[MAXN];  //判断是否被打击掉

vector<int> g[MAXN];

int tot;  // 连通块数量

int find(int x) {
    if (x != f[x]) {
        f[x] = find(f[x]);
    }
    return f[x];
}

void merge(int a, int b) {
    int fa = find(a);
    int fb = find(b);
    if (fa != fb)  //且之前没有连通
    {
        f[fb] = fa;
        tot--;  //合并这两个点并在总数减去一个
    }
}

/*
并查集，逆序做（即先假设给的k个星球全都被炸，求出此时的联通块个数，
就是经过k次打击的联通块个数。然后再加上最后一个被炸的星球，就求出了
经过k-1次打击的联通块个数。。。以此类推，最后把所有点都加进去，
就求出了经过0次打击后联通块个数）
*/
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, m, k, x, y, u;
    cin >> n >> m;
    for (int i = 0; i < n; ++i) {
        f[i] = i;
    }
    for (int i = 0; i < m; ++i) {
        cin >> x >> y;
        g[x].push_back(y);
        g[y].push_back(x);
    }
    cin >> k;
    tot = n - k;  //打击k次后所剩下的点
    for (int i = 1; i <= k; i++) {
        cin >> h[i];
        vis[h[i]] = true;  //被打击掉后就true，并把打击的点存储到h中
    }

    for (int i = 0; i < n; i++) {
        if (vis[i]) {
            continue;
        }
        for (auto to : g[i]) {
            if (vis[to]) {
                continue;
            }
            merge(i, to);  //如果都没有被打击
        }
    }

    ans[k + 1] = tot;  //这时为打击k次之后所剩下的连通块
    
    for (int i = k; i >= 1; i--) {
        ++tot;
        vis[h[i]] = false;
        for (auto to : g[h[i]]) {
            if (vis[to]) continue;
            merge(h[i], to);
        }
        ans[i] = tot;
    }

    for (int i = 1; i <= k + 1; ++i) {
        cout << ans[i] << endl;
    }

    return 0;
}

