#include <bits/stdc++.h>
using namespace std;

int ans[1002];

int N;
int legal_array(int n){
    if (n == 1){
        return 1;
    }
    if (n > 1){
        if (ans[n] > 0){
            return ans[n];
        }
        int temp = 1;
        for (int i = 1; i <= n / 2; ++i){
            temp += legal_array(i);
        }
        ans[n] = temp;
        return temp;
    }
}

int main () {
    cin >> N;
    cout << legal_array(N);
}