#include <bits/stdc++.h>
#define LL long long
using namespace std;

struct interval{
    int l, r;
} a[100010];

bool cmp (const interval& a, const interval& b) {
    return a.l < b.l;
}

int n;
int s[200010];
LL f[100010];
const int MOD = 1e9 + 7;

int getKSM(int a, int b) {
    int ans = 1;
    while (b > 0) {
        if (b % 2 == 1) {  // ���bΪ����
            ans = (1LL * ans * a) % MOD;
        }
        a = (1LL * a * a) % MOD;
        b /= 2;
    }
    return ans % MOD;
}

int main () {
    freopen ("help.in", "r", stdin);
    freopen ("help.out", "w", stdout);
    cin >> n;
    for (int i = 1; i <= n; ++i) {
        cin >> a[i].l >> a[i].r;
        s[a[i].r]++;
    }
    sort(a + 1, a + n + 1, cmp);
    for (int i = 1; i <= 2 * n; ++i) {
        s[i] += s[i - 1];
    }
    for (int i = 1; i <= n; ++i) {
        f[i] = (2 * f[i - 1] + getKSM(2, s[a[i].l - 1])) % MOD;
    }
    cout << f[n];
}