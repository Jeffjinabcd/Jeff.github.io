#include <bits/stdc++.h>
using namespace std;

int N;
int l[200005];
int r[200005];
int b[200005];

void dfs (int x, int d) {
    if (x == 0) {
        cout << d / 2 << endl;
        return;
    }
    if (d % 2 == 0) {
        dfs(l[x], d + 1);
        dfs(r[x], d); 
    } else {
        dfs(l[x], d);
        dfs(r[x], d + 1);
    }
}

int main () {
    cin >> N;
    for (int i = 1; i <= N; i++) {
        cin >> b[i];
        if (b[i] > b[i - 1]) {
            r[b[i - 1]] = b[i];
        }
        else {
            l[b[i - 1]] = b[i];
        }
    }
    dfs(b[1], 0);
    return 0;
}