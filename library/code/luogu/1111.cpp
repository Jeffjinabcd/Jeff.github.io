#include <bits/stdc++.h>
using namespace std;

struct line{
    int s, e, t;
};

bool cmp(const line &a, const line &b){
    return a.t < b.t;
}

line lines[100010];
int f[1010];

int find (int x){
    if (f[x] == x){
        return x;
    }

    f[x] = find(f[x]);
    return f[x];
}

bool merge (int x, int y){
    x = find(x);
    y = find(y);
    if (x != y){
        f[x] = y;
        return true;
    }
    return false;
}

int n, m;

int main (){
    cin >> n >> m;
    for (int i = 0; i < n; ++i){
        f[i] = i;
    }
    for (int i = 0; i < m; ++i){
        line temp;
        cin >> temp.s >> temp.e >> temp.t;
        lines[i] = temp;
    }
    sort(lines, lines + m, cmp);
    int cnt = 0;
    for (int i = 0; i < m; ++i){
        if (merge(lines[i].s, lines[i].e)){
            ++cnt;
        }
        if (cnt == n - 1){
            cout << lines[i].t;
            return 0;
        }
    }
    cout << "-1";
}