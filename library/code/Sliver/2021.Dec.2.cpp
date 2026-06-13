#include <bits/stdc++.h>
using namespace std;
int n, m, t;
int f[100005];
vector<int> v[100005];

int find(int x) {
    if (x == f[x]) {
        return x;
    }
    f[x] = find(f[x]);
    return f[x];
}

void merge(int u, int v) {
    int fu = find(u);
    int fv = find(v);
    if (fu != fv) {
        f[fu] = fv;
    }
}

/*
先使用并查集维护好点与点之间的连接情况
把每个点存到它对应根节点的vector或arraylist中
再把1和n所在连通块的节点分别存好
枚举用哪一个联通块把1和n所在的联通块连接起来（可以利用二分查找）
*/
int main() {
    cin >> t;
    for (int i = 1; i <= t; i++) {
        cin >> n >> m;
        for (int j = 1; j <= n; j++) {
            f[j] = j;
        }
        for (int j = 1; j <= m; j++) {
            int a, b;
            cin >> a >> b;
            merge(a, b);
        }
        int f1 = find(1), fn = find(n);
        if (f1 == fn) {
            cout << 0 << endl;
            continue;
        }

        for (int j = 1; j <= n; j++) v[j].clear();
        for (int j = 1; j <= n; j++) v[find(j)].push_back(j);

        long long ans = LONG_MAX;
        for (int j = 2; j < n; j++) {
            if (find(j) != j) {
                continue;
            }
            int min1 = n, min2 = n;
            for (auto k : v[j]) {  // 枚举j所在连通块里的所有点
                int pos = lower_bound(v[f1].begin(), v[f1].end(), k) - v[f1].begin();
                if (pos < v[f1].size()) {
                    // 把k号点和1号点所在连通块中大于等于k大且最接近它的点连接
                    min1 = min(min1, v[f1][pos] - k);
                }
                if (pos > 0) {
                    // 把k号点和1号点所在连通块中比k小且最接近它的点连接
                    min1 = min(min1, k - v[f1][pos - 1]);
                }

                pos = lower_bound(v[fn].begin(), v[fn].end(), k) - v[fn].begin();
                if (pos < v[fn].size())
                    // 把k号点和n号点所在连通块中大于等于k大且最接近它的点连接
                    min2 = min(min2, v[fn][pos] - k);
                if (pos > 0) {
                    // 把k号点和n号点所在连通块中比k小且最接近它的点连接
                    min2 = min(min2, k - v[fn][pos - 1]);
                }
            }
            ans = min(ans, 1LL * min1 * min1 + 1LL * min2 * min2);
        }
        cout << ans << endl;
    }
    return 0;
}

/*
2
5 2
1 2
4 5
5 3
1 2
2 3
4 5
*/
