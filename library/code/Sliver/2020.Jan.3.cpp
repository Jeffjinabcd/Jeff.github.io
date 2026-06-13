#include <bits/stdc++.h>
#define LL long long
using namespace std;

const int N = 1e5 + 1;

struct edge {
    int u, v, w;
};

int n, m;
int dest[N];
edge e[N];
int f[N];

bool cmp(const edge &a, const edge &b) {
    return a.w < b.w;
}

int find(int x) {
    if (f[x] == x) {
        return x;
    }
    return f[x] = find(f[x]);
}

void merge(int u, int v) {
    int fu = find(u);
    int fv = find(v);
    if (fu != fv) {
        f[fu] = fv;
    }
}

bool check(int mid) {
    for (int i = 1; i <= n; i++) {
        f[i] = i;
    }

    for (int i = mid; i <= m; i++) {
        merge(e[i].u, e[i].v);
    }

    for (int i = 1; i <= n; i++) {
        if (find(i) != find(dest[i])) {
            return false;
        }
    }
    return true;
}

int main() {
    freopen("wormsort.in", "r", stdin);
    freopen("wormsort.out", "w", stdout);

    cin >> n >> m;

    bool isSorted = true;
    for (int i = 1; i <= n; i++) {
        scanf("%d", &dest[i]);
        if (dest[i] != i) {
            isSorted = false;
        }
    }
    // 特判  如果奶牛们不需要用任何虫洞来排序，输出 -1
    if (isSorted) {
        cout << -1 << endl;
        return 0;
    }

    for (int i = 1; i <= m; i++) {
        int u, v, w;
        scanf("%d%d%d", &u, &v, &w);
        e[i] = {u, v, w};
    }

    sort(e + 1, e + 1 + m, cmp);

    int left = 1, right = m, answer = 0;
    while (left <= right) {
        int mid = (left + right) / 2;
        if (check(mid)) {
            answer = e[mid].w;
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    cout << answer << endl;

    return 0;
}
