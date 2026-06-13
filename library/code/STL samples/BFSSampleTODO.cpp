#include<bits/stdc++.h>
using namespace std;

struct point {
    int x, y, s; // 行，列，步数 
};

queue<point> q;
int maze[51][51];
int dir[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};  // 方向数组 
bool vis[51][51];

/*
输入 
5 4
0 0 1 0
0 0 0 0
0 0 1 0
0 1 0 0
0 0 0 1
1 1 4 3

输出
7
*/ 
int main() {
    int row, col;
    cin>>row>>col;
    for(int i=1; i<=row; i++) {
        for(int j=1; j<=col; j++) {
            cin>>maze[i][j];
        }
    }
    int sx, sy, ex, ey;
    cin>>sx>>sy>>ex>>ey;

    q.push({sx, sy, 0}); // 星星之火, 可以燎原
    vis[sx][sy] = true;

    while(!q.empty()) {
        // 取出队首的点 
        int tx = q.front().x, ty = q.front().y, ts = q.front().s;
        q.pop();
        for(int i=0; i<4;i++) {
        	//算出新的位置坐标 
            int x = tx + dir[i][0], y = ty + dir[i][1];
            //判断新的位置是否越界 
            if (x < 1 || x > row || y < 1 || y > col){
                continue;
            }
            // 如果新的位置是障碍 或者 已走过
            if (vis[x][y] == true || maze[x][y] == 1){
                continue;
            }
            // 把新位置加入队列
            q.push({x, y, ts + 1});
            vis[x][y] = true;
            // 新的位置是否为终点
            if (x == ex && y == ey){
                cout << ts + 1 << endl;
                return 0;
            }
        }
    }
    cout<<-1<<endl;

    return 0;
}
