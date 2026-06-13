#include <bits/stdc++.h>
#define ll long long
using namespace std;


const int MAXN = 1e6 + 10;
ll c[MAXN];
ll ans[MAXN];
int n;

int lowbit(int x) {
	return x&(-x);
}

void update(int x, int v) {
	for(int i=x;i<=MAXN;i+=lowbit(i))
        c[i]+=v;
}

ll getSum(int x) {
	ll ans=0;
	for(int i=x; i>0; i-=lowbit(i))
		ans+=c[i];
	return ans;
}


int main () {
    cin >> n;
    for (int i = 0; i < n; ++i) {
        int tempx, tempy;
        cin >> tempx >> tempy;
        ++tempx;
        ++ans[getSum(tempx)];
        update(tempx, 1);
    }
    for (int i = 0; i < n; ++i){
        cout << ans[i] << endl;
    }
}