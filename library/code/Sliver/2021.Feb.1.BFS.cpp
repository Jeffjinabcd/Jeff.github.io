#include <bits/stdc++.h>
using namespace std;

struct point {
    int x, y;
};

const int DIR[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

int pasture[3500][3500];  // pasture[i][j] = 0:нчеё  1:спеё
int ans = 0;

queue<point> q;

bool comfortable(int x, int y) {
    if (pasture[x][y] == 0) {
        return false;
    }

    int adjacent = 0;
    for (int i = 0; i < 4; i++) {
        int nx = x + DIR[i][0];
        int ny = y + DIR[i][1];
        adjacent += pasture[nx][ny];
    }
    return adjacent == 3;
}

void check(int x, int y) {
    if (comfortable(x, y)) {
        q.push({x, y});
    }

    for (int i = 0; i < 4; i++) {
        int nx = x + DIR[i][0];
        int ny = y + DIR[i][1];
        if (comfortable(nx, ny)) {
            q.push({nx, ny});
        }
    }
}

void add(int x, int y) {
    pasture[x][y] = 1;
    check(x, y);

    while (!q.empty()){
        point p = q.front();
        q.pop();
        if (!comfortable(p.x, p.y)){
            continue;
        }
        for (int i = 0; i < 4; i++){
            if (pasture[p.x + DIR[i][0]][p.y + DIR[i][1]] == 0){
                ++ans;
                pasture[p.x + DIR[i][0]][p.y + DIR[i][1]] = 1;
                check(p.x + DIR[i][0], p.y + DIR[i][1]);
                break;
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    int n, x, y;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin>>x>>y;
        x += 1000;
        y += 1000;
        if (pasture[x][y] > 0) {
            ans--;
        } else {
            add(x, y);
        }
        cout << ans << endl;
    }

    return 0;
}