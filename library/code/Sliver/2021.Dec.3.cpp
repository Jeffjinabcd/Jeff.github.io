#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define N 5010
ll n, m; 
ll a[N], b[N], x[N], y[N], c[N*2]; 

int main() {
	cin >> n >> m;
	for(ll i = 1; i <= n; ++i) {
        ll j, k;
        cin >> j >> k;
        x[j]++, y[k]++;
    }
	for(ll i = 0; i <= m; ++i) {
		for(ll j = 0; j <= m; ++j){
			c[i + j] += x[i] * x[j];
            c[i + j + 1] -= y[i] * y[j];
        }
    }
    ll k = 0;
	for(ll i = 0; i <= 2*m; ++i)
    {
        k += c[i];
        cout << k << endl;
    }
}
