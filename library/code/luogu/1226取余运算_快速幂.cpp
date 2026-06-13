#include <bits/stdc++.h>
using namespace std;

int b, p, k;

/*
原理：a*b%k=(a%k)*(b%k)%k
对于任何一个自然数：p=2*(p/2)+p%2
*/
int f(int p) {
    if (p == 0)  // b^0%k=1
        return 1;
    int tmp = f(p / 2) % k;
    tmp = (1LL * tmp * tmp) % k;  // b^p%k=(b^(p/2))^2%k
    if (p % 2 == 1)
        tmp = (1LL * tmp * b) % k;  // 如果p为奇数，则b^p%k=((b^(p/2))^2)*b%k
    return tmp;
}

int main() {
    cin >> b >> p >> k;
    int tmpb = b;
    b %= k;
    printf("%d^%d mod %d=%d", tmpb, p, k, f(p));
    return 0;
}
