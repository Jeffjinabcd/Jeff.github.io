#include <bits/stdc++.h>
using namespace std;

int N, C;
long long res;
int n[200010];

int main () {
    cin >> N >> C;
    for (int i = 0; i < N; ++i){
        cin >> n[i];
    }
    sort(n, n + N);
    for (int i = 0; i < N; ++i){
        int A = n[i] + C;
        res += upper_bound(n + i, n + N, A) - lower_bound(n + i, n + N, A);
    }
    cout << res;
}
