#include <bits/stdc++.h>
using namespace std;

int N, n[100010], in[100010], res[100010], p, endd;
bool vis[100010];

struct point{
    int pos, add;
};

vector<point> line[100010];
vector<int> temp_line;
vector<int> temp_circle;
void find(int x) {
    if (vis[x]){
        return;
    }
    temp_line.push_back(x);
    vis[x] = true;
    --in[n[x]];
    endd = x;
    if (in[n[x]] == 0){
        find(n[x]);
    }
    return;
}

void circle(int x){
    int pos = x;
    while (!vis[pos]) {
        temp_circle.push_back(pos);
        vis[pos] = true;
        pos = n[pos];
    }
    return;
}


// for (auto &k : line[temp_circle[j]]){
//                     res[k.pos] = len + k.add;
//                     for (auto &m : line[k.pos]){
//                         res[m.pos] = len + k.add + m.add;
//                     }
//                 }
void addres(int pos){
    
}

int main () {
    // freopen("testing.in", "r", stdin);
    cin >> N;
    for (int i = 1; i <= N; ++i) {
        cin >> n[i];
        ++in[n[i]];
    }
    for (int i = 1; i <= N; ++i) {
        if (in[i] == 0){
            find(i);
            reverse(temp_line.begin(), temp_line.end());
            for (int i = 0; i < temp_line.size(); ++i){
                line[n[endd]].push_back({temp_line[i], i + 1});
            }
            temp_line.clear();
            endd = 0;
        }
    }
    for (int i = 1; i <= N; ++i){
        if (!vis[i]){
            circle(i);
            int len = temp_circle.size();
            for (int j = 0; j < len; ++j){
                res[temp_circle[j]] = len;
                for (auto &k : line[temp_circle[j]]){
                    res[k.pos] = len + k.add;
                    for (auto &m : line[k.pos]){
                        res[m.pos] = len + k.add + m.add;
                    }
                }
            }
            temp_circle.clear();
        }
    }
    for (int i = 1; i <= N; ++i){
        cout << res[i] << endl;
    }
}