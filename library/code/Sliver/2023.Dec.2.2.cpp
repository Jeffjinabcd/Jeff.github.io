#include <bits/stdc++.h>
using namespace std;

int N, K;
map<int, int> a, b;
int m1[500010], m2[500010];
int main () {
    cin >> N >> K;
    for (int i = 1; i <= K; ++i) {
        int temp;
        cin >> temp;
        a[temp] = i;
    }
    for (int i = 1; i <= K; ++i) {
        int temp;
        cin >> temp;
        b[temp] = i;
    }
    int inter = 0, out = 0;
    for (int i = 1; i <= N; ++i) {
        if (a[i] == 0 && b[i] == 0) {
            ++out;
        }
        if (a[i] != 0 && b[i] != 0) {
            int sum = (a[i] + b[i]) % K;
            int diff = (a[i] - b[i] + K) % K;
            ++ m1[sum], ++ m2[diff];
            inter = max(inter, m1[sum]);
            inter = max(inter, m2[diff]);
        }
    }
    cout << inter + out;
}