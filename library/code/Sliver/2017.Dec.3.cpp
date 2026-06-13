#include <bits/stdc++.h>
using namespace std;

int N, path[100002], inDegree[100002];

queue<int> q;

int ans;

void shuffle () {
    while (!q.empty()) {
        int temp = q.front();
        q.pop();
        ++ans;
        --inDegree[path[temp]];
        if (inDegree[path[temp]] == 0){
            q.push(path[temp]);
        }
    }
}

int main () {
    // freopen("testing.in", "r", stdin);
    freopen("shuffle.in", "r", stdin);
    freopen("shuffle.out", "w", stdout);
    cin >> N;
    for (int i = 1; i <= N; ++i) {
        cin >> path[i];
        ++inDegree[path[i]];
    }
    for (int i = 1; i <= N; ++i) {
        if (inDegree[i] == 0){
            q.push(i);
        }
    }
    shuffle();
    cout << N - ans;
}