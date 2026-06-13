#include <bits/stdc++.h>
using namespace std;

struct boot{
    int depth, step;
};
boot boots[251];
int N, B, best = INT_MAX, tiles[251];

bool vis[250][250];

void search(int t, int current){
    if (vis[t][current]){
        return;
    }
    else{
        vis[t][current] = true;
    }
    if (current >= best || current >= B - 1){
        return;
    }
    if (t == N - 1){
        best = min(current, best);
        return;
    }
    int limit = min(t + boots[current].step, N - 1);
    for (int j = t + 1; j <= limit; ++j){
        if (tiles[j] <= boots[current].depth){
            search(j, current);
        }
    }
    for (int j = current + 1; j <= B - 1; ++j){
        if (boots[j].depth >= tiles[t]){
            search(t, j);
        }
    }
    return;
}


int main () {
    // freopen("testing.in", "r", stdin);
    freopen("snowboots.in", "r", stdin);
    freopen("snowboots.out", "w", stdout);
    cin >> N >> B;
    for (int i = 0; i < N; ++i){
        cin >> tiles[i];
    }
    for (int i = 0; i < B; ++i){
        cin >> boots[i].depth >> boots[i].step;
    }
    
    search(0, 0);
    cout << best;
}