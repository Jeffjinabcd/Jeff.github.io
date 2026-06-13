#include <bits/stdc++.h>
using namespace std;

struct point {
    int x, y;
};

vector<point> switches[101][101];
bool rooms[101][101] = {};
bool vis[101][101] = {};

int dir[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

int main(){
    // freopen("testing.in", "r", stdin);
	freopen("lightson.in", "r", stdin);
	freopen("lightson.out", "w", stdout);
    int N, M;
    cin >> N >> M;
    rooms[1][1] = true;
    vis[1][1] = true;
    for (int i = 0; i < M; ++i){
        int temp1, temp2, temp3, temp4;
        cin >> temp1 >>  temp2 >> temp3 >> temp4;
        switches[temp1][temp2].push_back({temp3, temp4});
    }
    queue<point> current;
    current.push({1, 1});
    int ans = 1;
    while (!current.empty()){
        int x = current.front().x, y = current.front().y;
        // cout << x << " " << y << endl;
        current.pop();
        for (auto &k : switches[x][y]){
            if (!rooms[k.x][k.y]){
                ++ans;
                rooms[k.x][k.y] = true;
                for (int i = 0; i < 4; ++i){
                    int tx = k.x + dir[i][0], ty = k.y + dir[i][1];
                    if (tx < 1 || tx > N || ty < 1 || ty > N){
                        continue;
                    }
                    if (vis[tx][ty]){
                        current.push({k.x, k.y});
                        vis[k.x][k.y] = true;
                        break;
                    }
                }
            }
        }
        for (int i = 0; i < 4; ++i){
            int tx = x + dir[i][0], ty = y + dir[i][1];
            if (tx < 1 || tx > N || ty < 1 || ty > N){
                continue;
            }
            if (rooms[tx][ty] == false || vis[tx][ty] == true){
                continue;
            }
            vis[tx][ty] = true;
            current.push({tx, ty});
        }
    }
    cout << ans;
}