#include <bits/stdc++.h>
using namespace std;

int ksm(int x,int k, int mod){
    int a=1;
    while(k) {
    	if (k&1) // 判断k是否为奇数
		    a= 1LL*a*x%mod;
		k>>=1;
		x=1LL*x*x%mod;
	}        
    return a;
}

// 核心知识：同余定理和快速幂, 都是比较基础的数论。 
// 同余定理：（a + b） % n = ((a % n ) + (b % n)) % n
//            ab % n = (a % n) (b % n) % n
// 我们可以用式子表示出 x号小朋友最后的位置：x = (x + m * 10^k) % n 
// 恒等变形 x=(x+m*(10^k % n) % n) % n
int main() {
//	freopen("circle.in", "r", stdin);
//	freopen("circle.out", "w", stdout);
    int n, m, k,x;
    cin>>n>>m>>k>>x;
    cout<<(x+m*ksm(10, k, n)%n)%n;

    return 0;
}
