#include <bits/stdc++.h>
using namespace std;

int main () {
    // freopen("testing.in", "r", stdin);
    int N, M;
    cin >> N >> M;
    int temp = 0, result = 0, temp1;
    for (int i = 0; i < N; ++i){
        cin >> temp1;
        if (temp + temp1 <= M){
            temp += temp1;
        }
        else {
            temp = temp1;
            ++result;
        }
    }
    cout << result + 1;
}