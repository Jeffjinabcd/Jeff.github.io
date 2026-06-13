#include <bits/stdc++.h>
using namespace std;

struct blob {
    int size, peri;
};

struct pos {
    int x, y;
};

bool chart[1001][1001];
bool vis[1001][1001];
blob ans;
int N;

int dir[4][2] = {{0, 1}, {1, 0}, {-1, 0}, {0, -1}};

blob search (int x, int y){
    queue<pos> record;
    record.push({x, y});
    vis[x][y] = true;
    int size = 1, peri = 0;
    while (!record.empty()){
        int x1 = record.front().x, y1 = record.front().y;
        record.pop();
        for (int i = 0; i < 4; ++i){
            int tx = x1 + dir[i][0], ty = y1 + dir[i][1];
            if (tx < 0 || tx >= N || ty < 0 || ty >= N){
                ++peri;
                continue;
            }
            if (!vis[tx][ty] && chart[tx][ty]){
                ++size;
                record.push({tx, ty});
                vis[tx][ty] = true;
            }
            if (!chart[tx][ty]){
                ++peri;
            }
        }
    }
    return {size, peri};
}

int main () {
    // freopen("testing.in", "r", stdin);
    freopen("perimeter.in", "r", stdin);
    freopen("perimeter.out", "w", stdout);
    cin >> N;
    for (int i = 0; i < N; ++i){
        for (int j = 0; j < N; ++j){
            char temp;
            cin >> temp;
            if (temp == '#'){
                chart[i][j] = true;
            }
        }
    }
    for (int i = 0; i < N; ++i){
        for (int j = 0; j < N; ++j){
            if (!vis[i][j] && chart[i][j]){
                blob temp = search(i, j);
                if (ans.size < temp.size){
                    ans = temp;
                }
                else if (ans.size == temp.size && ans.peri > temp.peri){
                    ans = temp;
                }
            }
        }
    }
    cout << ans.size << " " << ans.peri;
}