#include <bits/stdc++.h>
using namespace std;

int s, n, cnt;
long long v[100001*30], w[100001*30];
long long dp[2001];

int main(){
	scanf("%d%d", &s, &n);
	for(int i=1; i<=n; i++){
		//		scanf("%lld%lld%lld", &v[i], &w[i], &k[i]);
		long long vi, wi, ki;
		scanf("%lld%lld%lld", &vi, &wi, &ki);
		int t = 1;
		while (ki>=t) {
			v[++cnt]=vi*t;
			w[cnt]=wi*t;
			ki-=t;
			t*=2;
		}
		if(ki>0) {
			v[++cnt]=vi*ki;
			w[cnt]=wi*ki;
		}
	}
	for(int i=1; i<=cnt; i++){
		for(int j=s; j>=w[i]; j--){
			dp[j]=max(dp[j], dp[j-w[i]]+v[i]);
		}
	}
	
	printf("%lld", dp[s]);
}
