#include <bits/stdc++.h>
#define ll long long
using namespace std;

const int MAXN = 10000;
ll c[MAXN][MAXN];
int n, m;

int lowbit(int x) {
	return x&(-x);
}

void update(int x, int y, int v) {
	for(int i=x;i<=MAXN;i+=lowbit(i)) {
        for(int j=y;j<=MAXN;j+=lowbit(j)) {
            c[i][j]+=v;
        }
    }
}

ll getSum(int x, int y) {
	ll ans=0;
	for(int i=x; i>0; i-=lowbit(i)) {
	    for(int j=y; j>0; j-=lowbit(j)) {
		    ans+=c[i][j];
        }
    }
	return ans;
}

int main () {
    cin >> n >> m;
    int temp;
    while(cin >> temp) {
        if (temp == 1) {
            int x, y, z; 
            cin >> x >> y >> z;
            update(x, y, z);
        }
        else {
            int x1, y1, x2, y2;
            cin >> x1 >> y1 >> x2 >> y2;
            cout << getSum(x2, y2) - getSum(x2, y1 - 1) - getSum(x1 - 1, y2) + getSum(x1 - 1, y1 - 1) << endl;
        }
    }
}