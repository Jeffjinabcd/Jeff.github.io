#include <bits/stdc++.h>
using namespace std;
int m, n, chess[102][102], res = INT_MAX;
int dir[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

bool vis[102][102];
int golds[102][102];

void search(int x, int y, int gold, int color){
    if (golds[x][y] <= 0 || golds[x][y] > gold){
        golds[x][y] = gold;
    }
    else if (golds[x][y] <= gold){
        return;
    }
    if (gold > res){
        return;
    }
    for (int i = 0; i < 4; ++i){
        int nx = x + dir[i][0], ny = y + dir[i][1];
        if (nx < 0 || nx >= m || ny < 0 || ny >= m){
            continue;
        }
        if (vis[nx][ny]){
            continue;
        }
        if (chess[x][y] == 0 && chess[nx][ny] == 0){
            continue;
        }
        vis[nx][ny] = true;
        if (chess[nx][ny] == 0){
            search(nx, ny, gold + 2, chess[x][y]);
        }
        else if (color == chess[nx][ny]){
            search(nx, ny, gold, chess[nx][ny]);
        }
        else {
            search(nx, ny, gold + 1, chess[nx][ny]);
        }
        vis[nx][ny] = false;
    }
}

int main() {
    // freopen("testing.in", "r", stdin);
    cin >> m >> n;
    vis[0][0] = true;
    golds[m - 1][m - 1] = -1;
    for (int i = 0; i < n; ++i){
        int temp1, temp2, temp3;
        cin >> temp1 >> temp2 >> temp3;
        chess[temp1 - 1][temp2 - 1] = temp3 + 1;
    }
    search(0, 0, 0, chess[0][0]);
    cout << golds[m - 1][m - 1];
}