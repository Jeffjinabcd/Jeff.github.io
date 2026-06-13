#include <bits/stdc++.h>
using namespace std;

int n, a[101];

struct recipe{
	int cnt;
	int e[101];
};

recipe re[101];

bool check(int ans) {
	int final[101] = {};
	final[n] = ans;
	
	for(int i=n; i >= 1; i--){
		if(a[i] >= final[i]){
			continue;
		}
		if(re[i].cnt == 0) { // no relevent recipe
			return false;
		}
		int diff = final[i] - a[i];
		for(int j = 1; j<=re[i].cnt; j++) {
			final[re[i].e[j]] += diff;
		}
	}

	return true;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0), cout.tie(0);
	
	cin >> n;
	for(int i=1; i<=n; i++){
		cin>>a[i];
	}
	int k;
	cin>>k;
	
    int id;
	for(int i=1;i<=k;i++){
		cin>>id;
		cin>>re[id].cnt;
		for(int j=1; j<=re[id].cnt; j++){
			cin >> re[id].e[j];
		}
	}
	
	int ans = 0;
	while(true){
		if(check(ans+1)){
			ans++;
		} else {
			break;
		}
	}
	
	cout<<ans<<endl;
	
	return 0;
}

