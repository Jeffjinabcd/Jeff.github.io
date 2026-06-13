#include <bits/stdc++.h>
using namespace std;

struct point{
    int h, pos;
};

int N;
long long res;
stack<point> num;


int main () {
    cin >> N;
    for (int i = 0; i < N; ++i){
        int h;
        cin >> h;
        if (num.size() > 0 && h < num.top().h){
            res += 2;
            // cout << i + 1 << " " << num.top().pos + 1 << endl;
        }
        bool s = false;
        while (num.size() > 0 && h >= num.top().h){
            s = true;
            res += i - num.top().pos + 1;
            // cout << i + 1 << " " << num.top().pos + 1 << endl;
            num.pop();
        }
        if (s && num.size() > 0) {
            res += i - num.top().pos + 1;
        }
        num.push({h, i});
    }
    cout << res;
}