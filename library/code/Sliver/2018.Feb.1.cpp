#include <bits/stdc++.h>
using namespace std;

pair<int, int> record[100010];

int main (){
    // freopen("testing.in", "r", stdin);
    freopen("reststops.in", "r", stdin);
    freopen("reststops.out", "w", stdout);
    int L, N, f, b;
    cin >> L >> N >> f >> b;
    for (int i = 0; i < N; ++i){
        cin >> record[i].first >> record[i].second;
    }
    bool ismax[N] = {};
    int temp = 0;
    for (int i = N - 1; i >= 0; --i){
        if (temp < record[i].second){
            temp = record[i].second;
            ismax[i] = true;
        }
    }
    long long result = 0;
    long long temp1 = 0;
    for (int i = 0; i < N; ++i){
        if (ismax[i]){
            result += 1ll * (f - b) * record[i].second * (record[i].first - temp1);
            temp1 = record[i].first;
        }
    }
    cout << result;
}