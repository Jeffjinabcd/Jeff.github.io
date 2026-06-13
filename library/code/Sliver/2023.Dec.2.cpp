#include <bits/stdc++.h>
using namespace std;

int N, K;
map<int, int> a, b;
int dif[500010], sum[500010];

int main () {
    cin >> N >> K;
    for (int i = 1; i <= K; ++i){
        int temp;
        cin >> temp;
        a[temp] = i;
    }
    for (int i = 1; i <= K; ++i){
        int temp;
        cin >> temp;
        b[temp] = i;
    }
    int maxx = 0, out = 0;
    for (int i = 1; i <= N; ++i){
        if (a[i] != 0 && b[i] != 0){
            ++dif[(a[i] - b[i] + K) % K];
            ++sum[(a[i] + b[i]) % K];
            maxx = max(dif[(a[i] - b[i] + K) % K], max(sum[(a[i] + b[i]) % K], maxx));
        }
        if (a[i] == 0 && b[i] == 0){
            ++out;
        }
    }
    cout << maxx + out;
}