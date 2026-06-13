#include <bits/stdc++.h>
using namespace std;
#define ll long long

struct interval {
    int l, r;
}a[100010];

bool cmp(interval &a, interval &b) {
    return a.l < b.l;
}
int fastpow(int x,int k, int mod){
    int a=1;
    while(k) {
    	if (k&1) a= 1LL*a*x%mod;
		k>>=1;
		x=1LL*x*x%mod;
	}        
    return a;
}
int n;
int s[200010];
ll f[100010];
const int MOD = 1e9 + 7;

int main() {
    freopen("help.in", "r", stdin);
    freopen("help.out", "w", stdout);
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
        f[i] = f[i - 1] * 2 + fastpow(2, s[a[i].l], MOD) ;
        f[i] %= MOD;
    }
    cout << f[n] << endl;
    return 0;
}