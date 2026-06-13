#include <bits/stdc++.h>
using namespace std;

int f[1001];

int find(int v) {  // ??root
	if(f[v] == v) {
		return v;
	}

	f[v] = find(f[v]);  // ¡¤????? 
	return f[v];
}

void merge(int u, int v) {
	u = find(u);
	v = find(v);
	if(u != v) {
		f[v] = u;
	}
}

/*
????
10???????????
9??????????

10 9
1 2
3 4
5 2
4 6
2 6
8 7
9 7
1 6
2 4

???
3
*/
int main() {
	ios::sync_with_stdio(false);
    cin.tie(0);
	cout.tie(0);
    
	int n, m;
	cin>>n>>m;

	// ?????
	for(int i=1; i<=n; i++) {
		f[i] = i;
	}

	for(int i=1; i<=m; i++) {
		int x, y;
		cin>>x>>y;
		merge(x, y);
	}

	int ans = 0;
	for(int i=1; i<=n; i++) {
		if(f[i] == i) ans++;
	}

	cout<<ans<<endl;

    return 0;
}
