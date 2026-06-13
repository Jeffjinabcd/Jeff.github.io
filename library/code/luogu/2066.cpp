#include <bits/stdc++.h>
using namespace std;

int f[11][16];
int value[11][16]; 

void print(int i, int j) {
	if(i==0)
		return;
	
	for(int k=0; k<=j; k++) {
		if(f[i][j] == f[i-1][k] + value[i][j-k]) {
			print(i-1, k);
			cout<<i<<" "<<j-k<<endl;
			break;
		}
	}
}

/*
3 3
30 40 50
20 30 50
20 25 30
*/ 
int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);

    // f[i][j]: 前i个公司分配j台机器能得到的最大盈利
	// f[N][M]: 结果
	
	// f[i][j] = max{f[i-1][k] + value[i][j-k]}  0<=k<=j
	
	int n, m;
	cin>>n>>m;
	for(int i=1; i<=n; i++) {
		for(int j=1; j<=m; j++) {
			cin>>value[i][j]; 
		}
	}
	
	for(int i=1; i<=n; i++) {
		for(int j=1; j<=m; j++) {
			for(int k=0; k<=j; k++) {
				f[i][j] = max(f[i][j], f[i-1][k] + value[i][j-k]);
			}
		}
	}
	
	cout<<f[n][m]<<endl;
	
	print(n, m);

	return 0;
}
