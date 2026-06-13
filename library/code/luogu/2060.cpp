#include <bits/stdc++.h>

using namespace std;

int main(){
    // freopen("testing.in", "r", stdin);
    int result = 0;
    int x1, x2, y1, y2;
    int chart[5][5] = { {0,3,2,3,2},
                        {3,2,1,2,3},
                        {2,1,4,3,2},
                        {3,2,3,2,3},
                        {2,3,2,3,4} };
    cin >> x1 >> y1 >> x2 >> y2;
    int x = abs(x1 - x2);
    int y = abs(y1 - y2);
    while (x > 4 || y > 4){
        if (x < 0){
            x = -x;
        }
        if (y < 0){
            y = -y;
        }
        if (x < y){
            swap (x, y);
        }
        x -= 2;
        y -= 1;
        ++result;
    }
    cout << result + chart[x][y];
}