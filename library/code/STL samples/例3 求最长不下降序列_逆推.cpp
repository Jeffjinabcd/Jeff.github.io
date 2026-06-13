#include<bits/stdc++.h>
using namespace std;

int a[100], dp[100], trace[100];

/*
14
13 7 9 16 38 24 37 18 44 19 21 22 63 15

输出
max=8
7 9 16 18 19 21 22 63 
*/
int main()
{
	int n;
	cin>>n;
	for(int i=1; i<=n; i++)
	{
		cin>>a[i]; // 原始数据
		dp[i]=1;   // LIS 
		trace[i]=0; // 上一个数的位置
	}

	// 求LIS
	for(int i=n-1; i>=1; i--)
	{
		int maxlis=0;
		int maxindex=0;
		for(int j=i+1; j<=n; j++)
		{
			if(a[j]>a[i] && dp[j]>maxlis)
			{
				maxlis=dp[j];
				maxindex=j;
			}
		}
		
		if(maxlis>0)
		{
			dp[i]=maxlis+1;
			trace[i]=maxindex;
		}
	}

	int pos=1;
	for(int i=2; i<=n; i++)
	{
		if(dp[i]>dp[pos])
			pos=i;
	} 
	cout<<"max="<<dp[pos]<<endl;
	
	// 输出最长不下降序列
	while(pos!=0)
	{
		cout<<a[pos]<<" ";
		pos=trace[pos];
	}
	
    return 0;
}
