#include <bits/stdc++.h>
using namespace std;

int main () {
    // freopen("testing.in", "r", stdin);
    int n, p = 0, result = 0;
    cin >> n;
    int q[n];
    for (int i = 0; i < n; ++i){
        cin >> q[i];
        p += q[i];
    }
    p /= n;
    for (int i = 0; i < n; ++i){
        q[i] -= p;
    }
    for (int i = 0; i < n; ++i){
        if (q[i] != 0){
            q[i + 1] += q[i];
            ++result;
        }
    }
    cout << result;
}