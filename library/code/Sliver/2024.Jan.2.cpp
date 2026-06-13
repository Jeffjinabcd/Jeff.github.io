#include <bits/stdc++.h>
using namespace std;

int N, potions[100010], in[100010], out[100010], p[100010];
vector<int> tree[100010];

int leaf[100010];
int dp[100010];

bool vis[100010];

void dfs (int u, int f) {
    for (int v : tree[u]) {
        if (v != f) {
            dfs(v, u);
            leaf[u] += leaf[v];
        }
    }
    if (!leaf[u]){
        leaf[u] = 1;
    }
}

void dfsf (int u, int f) {
    for (int v : tree[u]) {
        if (v != f) {
            dfsf(v, u);
            dp[u] += dp[v];
        }
    }
    dp[u] = min(leaf[u], dp[u] + p[u]);
}


int main () {
    cin >> N;
    for (int i = 0; i < N; ++i){
        cin >> potions[i];
    }
    for (int i = 1; i < N; ++i){
        int temp1, temp2;
        cin >> temp1 >> temp2;
        tree[temp1].push_back(temp2);
        tree[temp2].push_back(temp1);
    }
    dfs(1, 0);
    for (int i = 0; i < leaf[1]; ++i) {
        ++p[potions[i]];
    }
    dfsf(1, 0);
    cout << dp[1];
}