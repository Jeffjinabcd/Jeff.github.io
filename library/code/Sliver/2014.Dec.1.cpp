#include <bits/stdc++.h>
using namespace std;

int B, E, P, N, M, ans = INT_MAX;

vector<int> record[40010];
bool vis[40010];

int Bs[40010];
int Es[40010];
int Ps[40010];

struct trail {
    int point, step;
};

void clearr(){
    for (int i = 0; i < 40010; ++i){
        vis[i] = false;
    }
}

void search(int current, char multiple){
    queue<trail> trails;
    trails.push({current, 0});
    while (!trails.empty()){
        int x = trails.front().point, y = trails.front().step;
        trails.pop();
        for (auto & k : record[x]){
            if (!vis[k]){
                vis[k] = true;
                trails.push({k, y + 1});
                if (multiple == 'B'){
                    Bs[k] = (y + 1) * B;
                }
                else if (multiple == 'E'){
                    Es[k] = (y + 1) * E;
                }
                else if (multiple == 'P'){
                    Ps[k] = (y + 1) * P;
                }
            }
        }
    }
    clearr();
}

int main () {
    fill_n(Bs, 40010, INT_MAX);
    fill_n(Es, 40010, INT_MAX);
    fill_n(Ps, 40010, INT_MAX);
    // freopen("testing.in", "r", stdin);
    freopen("piggyback.in", "r", stdin);
    freopen("piggyback.out", "w", stdout);
    cin >> B >> E >> P >> N >> M;
    for (int i = 0; i < M; ++i){
        int temp1, temp2;
        cin >> temp1 >> temp2;
        record[temp1].push_back(temp2);
        record[temp2].push_back(temp1);
    }
    search(1, 'B');
    search(2, 'E');
    search(N, 'P');
    Bs[1] = 0;
    Es[2] = 0;
    Ps[N] = 0;
    for (int i = 1; i <= N; ++i){
        ans = min(ans, Bs[i] + Es[i] + Ps[i]);
    }
    cout << ans;
}