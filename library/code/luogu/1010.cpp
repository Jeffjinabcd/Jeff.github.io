#include <bits/stdc++.h>
using namespace std;

int N;


void split(int n){
    if (n == 1){
        cout << "2(0)";
    }
    else if (n == 2){
        cout << "2";
    }
    else {
        int t = 1, exp = 0;
        while (t <= n){
            t *= 2;
            ++exp;
        }
        t /= 2;
        --exp;
        n -= t;
        cout << "2";
        if (exp != 1){
            cout << "(";
            split(exp);
            cout << ")";
        }
        if (n > 0){
            cout << "+";
            split(n);
        }
    }
    
}
int main () {
    cin >> N;
    split(N);
}