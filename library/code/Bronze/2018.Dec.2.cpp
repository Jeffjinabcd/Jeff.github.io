#include <bits/stdc++.h>
using namespace std;

int main (){
    //freopen("blist.in", "r", stdin);
    //freopen("blist.out", "w", stdout);
    vector<int> buckets (1001);
    int N, s, t, b;
    cin >> N;
    for (int i = 0; i < N; ++i){
        cin >> s >> t >> b;
        for (int j = s; j <= t; ++j){
            buckets[j] += b;
        }
    }
    int result = 0;
    for (int i = 0; i < 1001; ++i){
        if (buckets[i] > result){
            result = buckets[i];
        }
    }
    cout << result;
}
