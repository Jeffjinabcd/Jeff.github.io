#include <bits/stdc++.h>
using namespace std;

/*
原理：a*b%k=(a%k)*(b%k)%k
对于任何一个自然数：p如果是偶数，p=2*(p/2)，p如果是奇数，p=2*(p/2)+1
*/
int getKSM(int a, int b, int k) {
    int ans = 1;
    while (b > 0) {
        if (b % 2 == 1) {  // 如果b为奇数
            ans = (1LL * ans * a) % k;
        }
        a = (1LL * a * a) % k;
        b /= 2;
    }
    return ans % k;
}

int main() {
    int b, p, k;
    cin >> b >> p >> k;
    int tmpb = b;
    b %= k;
    printf("%d^%d mod %d=%d", tmpb, p, k, getKSM(b, p, k));
    return 0;
}
