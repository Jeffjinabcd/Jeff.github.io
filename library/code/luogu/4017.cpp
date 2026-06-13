#include <bits/stdc++.h>
using namespace std;

int n, m;
vector<int> a[5010];
int in[5010];
int out[5010];
long long level[5010];
queue<int> to;



int main () {
    // freopen("testing.in", "r", stdin);
    cin >> n >> m;
    for (int i = 1; i <= m; ++i){
        int temp1, temp2;
        cin >> temp1 >> temp2;
        a[temp2].push_back(temp1);
        ++out[temp2];
        ++in[temp1];
    }
    for (int i = 1; i <= n; ++i){
        if (in[i] == 0){
            to.push(i);
            level[i] = 1;
        }
    }
    while (!to.empty()){
        int current = to.front();
        to.pop();
        for (int i = 0; i < out[current]; ++i){
            --in[a[current][i]];
            level[a[current][i]] = (level[current] + level[a[current][i]]) % 80112002;
            if (in[a[current][i]] == 0){
                to.push(a[current][i]);
            }
        }
    }
    long long ans = 0;
    for (int i = 1; i <= n; ++i){
        if (out[i] == 0){
            ans = (ans + level[i]) % 80112002;
        }
    }
    cout << ans;
}