#include <bits/stdc++.h>
using namespace std;

int N, ord[201], q[201], ans;
struct point {
    int x, y;
} p[201];

bool cmpx(const point &p1, const point &p2) { return p1.x < p2.x; }

bool cmpy(int a, int b) { return p[a].y < p[b].y; }

/*
枚举我们需要的点集，找到围住点集最小的矩形
判断矩形是否能够扩张成为一个正方形，能的话就计入答案贡献。
*/
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    cin >> N;
    for (int i = 1; i <= N; i++) cin >> p[i].x >> p[i].y;

    sort(p + 1, p + N + 1, cmpx);

    for (int i = 1; i <= N; i++) ord[i] = i;
    sort(ord + 1, ord + N + 1, cmpy);

    ans = N + 1;  // 单点集合 + 空集
    for (int l = 1; l < N; l++) {   // 左边界
        for (int r = l + 1; r <= N; r++) { // 右边界
            int cnt = 0, u = 0, v = 0; // 左右边界的点对应的上边界u和下边界v
            for (int i = 1; i <= N; i++)
                if (ord[i] >= l && ord[i] <= r) {
                    q[++cnt] = ord[i]; // y值小的在前
                    if (ord[i] == l) u = cnt;
                    if (ord[i] == r) v = cnt;
                }

            if (u > v) swap(u, v);

            for (int i = 1; i <= u; i++) {
                int cl = l - 1, cr = r + 1;
                for (int j = cnt; j >= v; j--) {
                    while (cl >= 1 && (p[cl].y < p[q[i]].y || p[cl].y > p[q[j]].y)) {
                        cl--;
                    }
                    while (cl <= N && (p[cr].y < p[q[i]].y || p[cr].y > p[q[j]].y)) {
                        cr++;
                    }
                    if (cl >= 1 && cr <= N && p[q[j]].y - p[q[i]].y > p[cr].x - p[cl].x - 2) {
                        continue;
                    }
                    if (i > 1 && j < cnt && p[r].x - p[l].x > p[q[j - 1]].y - p[q[i - 1]].y - 2) {
                        continue;
                    }

                    ans++;
                }
            }
        }
    }

    cout << ans << endl;
    return 0;
}
