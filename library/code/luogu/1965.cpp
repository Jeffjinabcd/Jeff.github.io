#include <bits/stdc++.h>
using namespace std;

int getKSM(int a, int b, int k) {
    int ans = 1;
    while (b > 0) {
        if (b % 2 == 1) {  // ���bΪ����
            ans = (1LL * ans * a) % k;
        }
        a = (1LL * a * a) % k;
        b /= 2;
    }
    return ans % k;
}

int main() {
    int n, m, k, x;
    cin >> n >> m >> k >> x;
    cout << (getKSM(10, k, n) * m + x) % n;
}
