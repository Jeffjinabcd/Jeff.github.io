#include <bits/stdc++.h>
using namespace std;
struct point {
    int x, y;
};

const int M = 32;
int n;
int a[M][M];
bool vis[M][M];
queue<point> q;
int dir[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

void bfs(int x, int y) {
    q.push({x, y});
    vis[x][y] = true;
    while (!q.empty()) {
        point p = q.front();
        q.pop();
        for (int i = 0; i < 4; i++) {
            int nx = p.x + dir[i][0];
            int ny = p.y + dir[i][1];
            if (nx < 0 || nx > n + 1 || ny < 0 || ny > n + 1) continue;

            if (a[nx][ny] == 0 && !vis[nx][ny]) {
                q.push({nx, ny});
                vis[nx][ny] = true;
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    cin >> n;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            cin >> a[i][j];
            if (a[i][j] == 1) {
                vis[i][j] = true;
            }
        }
    }

    bfs(0, 0);

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (!vis[i][j]) {
                cout << "2 ";
            } else {
                cout << a[i][j] << " ";
            }
        }
        cout << endl;
    }

    return 0;
}