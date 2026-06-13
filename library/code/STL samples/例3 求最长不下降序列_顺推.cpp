#include<bits/stdc++.h>
using namespace std;

int b[200][4];

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
		cin>>b[i][1];
		b[i][2]=1; // LIS 
		b[i][3]=0;
	}
	
	// 求LIS
	for(int i=2; i<=n; i++)
	{
		int maxLen=0;
		int idx=0;
		for(int j=1; j < i; j++)
		{
			if(b[j][1]<b[i][1] && b[j][2]>maxLen)
			{
				maxLen=b[j][2];
				idx=j;
			}
		}
		
		if(maxLen>0)
		{
			b[i][2]=maxLen+1;
			b[i][3]=idx;
		}
	}
	
	int pos=1;
	for(int i=2; i<=n; i++)
	{
		if(b[i][2]>b[pos][2])
			pos=i;
	} 
	cout<<"max="<<b[pos][2]<<endl;
	
	// 输出最长不下降序列
	stack<int> st;
	while(pos!=0)
	{
		st.push(b[pos][1]);
		pos=b[pos][3];
	}
	
	while(!st.empty()) {
		cout<<st.top()<<" ";
		st.pop();
	}
	 
	
    return 0;
}
