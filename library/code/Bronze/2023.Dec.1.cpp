#include <bits/stdc++.h>
using namespace std;

int main () {
    long long N, M;
    cin >> N >> M;
    vector<long long> c (N);
    for (int i = 0; i < N; ++i){
        cin >> c[i];
    }
    long long low = 0, high, temp;
    for (int i = 0; i < M; ++i){
        cin >> high;
        low = 0;
        for (int j = 0; j < N && low < high; ++j){
            if (low < c[j]){
                temp = c[j];
                c[j] += min(c[j] - low, high - low);
                low = temp;
            }
            
            
            
        }
    }
    for (int i = 0; i < N; ++i){
        cout << c[i] << endl;
    }
}