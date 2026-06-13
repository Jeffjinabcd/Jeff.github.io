#include <bits/stdc++.h>
using namespace std;
const int N = 101;
const int M = 10001;
int n, m, t, price[N][N], f[M];
int main()
{
	cin >> t >> n >> m;
	for(int i = 1; i <= t; i++)
		for(int j = 1; j <= n; j++)
			cin >> price[j][i];
          //读入每种商品每天的价格
	for(int k = 1; k < t; k++)
	{
		memset(f, 0, sizeof f);//每轮开始前都要制零
		for(int i = 1; i <= n; i++)
			for(int j = price[i][k]; j <= m; j++)//完全背包,正着循环
				f[j] = max(f[j], f[j - price[i][k]] + price[i][k + 1] - price[i][k]);
      
		m += f[m];//加上盈利的钱，进入下一轮买卖
	}
	cout << m;
	return 0;
}