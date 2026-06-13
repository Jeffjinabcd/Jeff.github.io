#include <bits/stdc++.h>
using namespace std;

struct pos {
    int x, y;
};

bool chart[2020][2020];
bool vis[2020][2020];
int N;

int dir[4][2] = {{0, 1}, {1, 0}, {-1, 0}, {0, -1}};

void cover (int x, int y){
    queue<pos> record;
    record.push({x, y});
    while (!record.empty()){
        int tx = record.front().x, ty = record.front().y;
        record.pop();
        // cout << tx << " " << ty << endl;
        for (int i = 0; i < 4; ++i){
            int x1 = tx + dir[i][0], y1 = ty + dir[i][1];
            if (x1 < 0 || x1 >= 2020 || y1 < 0 || y1 >= 2020){
                continue;
            }
            if (vis[x1][y1]){
                continue;
            }
            if (!chart[x1][y1]){
                record.push({x1, y1});
                vis[x1][y1] = true;
            }
        }
    }
}

int main (){
    // freopen("testing.in", "r", stdin);
    freopen("gates.in", "r", stdin);
    freopen("gates.out", "w", stdout);
    cin >> N;
    chart[1005][1005] = true;
    pos current = {1005, 1005};
    for (int i = 0; i < N; ++i){
        char temp;
        cin >> temp;
        if (temp == 'N'){
            chart[current.x][current.y - 1] = true;
            chart[current.x][current.y - 2] = true;
            current = {current.x, current.y - 2};
        }
        if (temp == 'W'){
            chart[current.x - 1][current.y] = true;
            chart[current.x - 2][current.y] = true;
            current = {current.x - 2, current.y};
        }
        if (temp == 'S'){
            chart[current.x][current.y + 1] = true;
            chart[current.x][current.y + 2] = true;
            current = {current.x, current.y + 2};
        }
        if (temp == 'E'){
            chart[current.x + 1][current.y] = true;
            chart[current.x + 2][current.y] = true;
            current = {current.x + 2, current.y};
        }
    }
    cover(0, 0);
    int areas = 0;
    for (int i = 5; i < 2015; ++i){
        for (int j = 5; j < 2015; ++j){
            if (!vis[i][j] && !chart[i][j]){
                cover(i, j);
                ++areas;
            }
        }
    }
    cout << areas;
}