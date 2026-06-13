#include <bits/stdc++.h>
using namespace std;

const int N = 1e5 + 1;

struct tile {
    int f, id;
};

struct boot {
    int s, d, id;
};

int n, b;
tile tiles[N];
int f[N];
int siz[N];
bool vis[N];
int ans[N];
int maxgap;
boot boots[N];

inline int find(int x) {
    if (f[x] == x) {
        return x;
    }
    return f[x] = find(f[x]);
}

inline void merge(int u, int v) {
    int fu = find(u);
    int fv = find(v);
    if (fu == fv) {
        return;
    }
    if (siz[fu] < siz[fv]) {
        swap(fu, fv);
    }
    f[fv] = fu;
    siz[fu] += siz[fv];
}

bool cmptile(tile &a, tile &b) { return a.f > b.f; }

bool cmpboot(boot &a, boot &b) { return a.s > b.s; }

int main() {
    freopen("snowboots.in", "r", stdin);
    freopen("snowboots.out", "w", stdout);
    scanf("%d%d", &n, &b);
    for (int i = 1; i <= n; i++) {
        int x;
        scanf("%d", &x);  // i�ŵ�ש�Ļ�ѩ���
        tiles[i] = {x, i};
        f[i] = i;
        siz[i] = 1; 
    }
    for (int i = 1; i <= b; i++) {
        int s, d;
        scanf("%d%d", &s, &d);  // ���ܵ�����ѩ��� ��󲽳�
        boots[i] = {s, d, i};
    }
    sort(tiles + 1, tiles + n + 1, cmptile);  // ����ѩ��ȴӴ�С����
    sort(boots + 1, boots + b + 1, cmpboot);  // ѥ�ӳ��ܶȴӴ�С����

    int tile_idx = 1;
    for (int i = 1; i <= b; i++) {
        while (tile_idx <= n && tiles[tile_idx].f > boots[i].s) {
            int current_tile_pos = tiles[tile_idx].id;
            vis[current_tile_pos] = true;
            if (current_tile_pos > 1 && vis[current_tile_pos - 1]) {
                merge(current_tile_pos, current_tile_pos - 1);
            }
            if (current_tile_pos < n && vis[current_tile_pos + 1]) {
                merge(current_tile_pos, current_tile_pos + 1);
            }
            maxgap = max(maxgap, siz[find(current_tile_pos)]);
            tile_idx++;
        }
        if (boots[i].d > maxgap) {
            ans[boots[i].id] = 1;
        } else {
            ans[boots[i].id] = 0;
        }
    }

    for (int i = 1; i <= b; i++) {
        cout << ans[i] << "\n";
    }

    return 0;
}