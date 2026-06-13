#include <bits/stdc++.h>
#define ll long long
using namespace std;

const int MAXN = 1e6 + 10;
ll c[MAXN];
ll ans[MAXN];
int n, m;

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
    cin >> n >> m;
    for (int i = 1; i <= n; ++i) {
        int temp;
        cin >> temp;
        update(i, temp);
    }
    for (int i = 0; i < m; ++i) {
        int temp1, temp2, temp3;
        cin >> temp1 >> temp2 >> temp3;
        if (temp1 == 1){
            update(temp2, temp3);
        }
        else {
            cout << getSum(temp3) - getSum(temp2 - 1);
        }
    }
}