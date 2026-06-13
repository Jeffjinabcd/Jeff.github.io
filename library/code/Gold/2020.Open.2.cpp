#include <bits/stdc++.h>
#define ll long long
using namespace std;

int N, M;
int p[200001];
int f[200001];
int ans[200001], cnt;

int find(int x) {
    if(f[x] == x) {
        return x;
    }
    f[x] = find(f[x]);
    return f[x];
}

void merge(int u, int v) {
    u = find(u);
    v = find(v);
    if (u != v) {
        f[u] = v;
        if (p[u] && p[v]) {
            merge(p[u], p[v]);
        }
        else if (p[u]) {
            p[v] = p[u];
        }
    }
}

int main() {
    freopen("fcolor.in", "r", stdin);
    freopen("fcolor.out", "w", stdout);
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);

    cin>>N>>M;
    for (int i = 1; i <= N; i++) {
        f[i] = i;
    }

    for (int i = 1; i <= M; i++) {
        int u, v;
        cin>>u>>v;
        u = find(u);
        if (!p[u]) {
            p[u] = v;
        }
        else {
            merge(v, p[u]);
        }
    }
    int index = 1;
    for (int i = 1; i <= N; i++) {
        int x = find(i);
        if (!ans[x]) {
            ans[x] = index;
            ++index;
        }
        cout << ans[x] << endl;
    }
    return 0;
}
