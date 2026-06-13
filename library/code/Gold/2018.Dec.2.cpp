#include <bits/stdc++.h>
#define LL long long
using namespace std;

map<string, LL> mp;
LL n, ans;
string a[6];

/*
总共对数-和谐对数，计算和谐对数利用容斥原理
和谐的对数可以分成5种情况: 1种口味相同 ~ 5种口味相同
*/
int main() {
    freopen("cowpatibility.in", "r", stdin);
    freopen("cowpatibility.out", "w", stdout);
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);

    cin>>n;
	ans=n*(n-1)/2; //总共对数 
	for (int c=1; c<=n; c++) {
	   	for (int i=1;i<=5;i++) {
            cin>>a[i];
        }
		sort(a+1,a+6);
		LL sum=0;
		for (int i=1; i<=31; i++) { //二进制枚举出所有情况
			int cnt = 0;
            string key = "";
            for (int j = 1; j <= 5; ++j) {
                if (i & (1 << (j - 1))) {
                    cnt++;
                    key += ' ' + a[j];
                }
            }

            if (cnt & 1) {
                sum += mp[key];
            }
            else {
                sum -= mp[key];
            }
            mp[key]++;
        }
        ans -= sum; // 总共对数-和谐对数
    }
    cout<<ans<<endl;

    return 0;
}