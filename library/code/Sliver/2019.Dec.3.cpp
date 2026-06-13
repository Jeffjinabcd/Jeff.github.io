#include <bits/stdc++.h>
using namespace std;
int n, m, f[100001];
string t;
int cnt;

int find(int v) {
    if (f[v] == v) return v;
    f[v] = find(f[v]);
    return f[v];
}

void merge(int u, int v) {
    int fu = find(u);
    int fv = find(v);
    if (fu != fv) {
        f[fv] = fu;
    }
}

int main() {
    freopen("milkvisits.in","r",stdin);
    freopen("milkvisits.out","w",stdout);
    ios::sync_with_stdio(false);
    cin.tie(0);
    cin >> n >> m >> t;
    for (int i = 1; i <= n; i++) {
        f[i] = i;
    }
    for (int i = 0; i < n - 1; i++) {
        int x, y;
        cin >> x >> y;
        if (t[x - 1] == t[y - 1]) {
            merge(x, y);
        }
    }
    for (int i = 0; i < m; i++) {
        int a, b;
        char c;
        cin >> a >> b >> c;
        if (find(a) == find(b) && t[a - 1] != c) {
            cout << 0;
        } else {
            cout << 1;
        }
    }
    return 0;
}
