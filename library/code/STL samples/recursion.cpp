#include <bits/stdc++.h>
using namespace std;

int ans[100];

int fibonacci (int n){
    if (ans[n] > 0){
        return ans[n];
    }
    if (n == 1 || n == 2){
        return 1;
    }
    ans[n] = fibonacci(n - 1) + fibonacci(n - 2);
    return ans[n];
}

int gcd (int a, int b){
    if (a % b == 0){
        return b;
    }
    return gcd(b, a % b);
}

int main () {
    cout << gcd (24, 18);
}