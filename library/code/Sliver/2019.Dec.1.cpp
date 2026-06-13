#include <bits/stdc++.h>
using namespace std;

int main() {
    // freopen("testing.in", "r", stdin);
    freopen("moobuzz.in", "r", stdin);
    freopen("moobuzz.out", "w", stdout);
    int n;
    cin >> n;
    bool check = true;
    int ird = 0, fth = 0, een = 0;
    int pre_ird, pre_fth;
    int N = n;
    while (check){
        pre_ird = ird;
        pre_fth = fth;
        ird = (N - (N % 3)) / 3;
        fth = (N - (N % 5)) / 5;
        een = (N - (N % 15)) / 15;
        if (ird - pre_ird > 0 || fth - pre_fth > 0){
            check = true;
        }
        else{
            check = false;
        }
        N = n + ird + fth - een;
    }
    cout << N;
}