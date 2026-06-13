#include<iostream>
#include<cstdio>
#include<cmath>
#include<string>
#include<algorithm>
#include<queue>
using namespace std;
bool f[21][21];//记录是否有路径相连
int a[21];//记录地雷数
int path[21],ans[21],cnt;//path记录路径，ans记录答案，cnt记录走了多少个点
bool b[21];//记录该点是否走过
int n;
int maxx;//记录挖的最大地雷数
bool chck(int x)//检查是否还能继续往下挖
{
	for(int i=1;i<=n;i++)
	{
		if(f[x][i]&&!b[i]) return false;
 	}
 	return true;
}
void dfs(int x,int stp,int sum)//x记录现在位置，stp记录走了几个点，sum记录挖的地雷数
{
	if(chck(x))
	{
		if(maxx<sum)//更新最大值和路径
		{
			maxx=sum;
			cnt=stp;
			for(int i=1;i<=stp;i++)
			ans[i]=path[i];	
		}
		return ;
	}
	for(int i=1;i<=n;i++)//寻找下一个能去的地方
	{
		if(f[x][i]&&!b[i])
		{
			b[i]=1;//标记走过
			path[stp+1]=i;//记录路径
			dfs(i,stp+1,sum+a[i]);
			b[i]=0;//回溯
		}
		
	}
}
int main()
{
	cin>>n;
	for(int i=1;i<=n;i++)
	cin>>a[i];
	for(int i=1;i<n;i++)
	for(int j=i+1;j<=n;j++)
	{
		cin>>f[i][j];//这里是单向边，题目没啥清楚，导致我调了半个小时；
	}
	for(int i=1;i<=n;i++)
	{
		b[i]=1;
		path[1]=i;//记录起点
		dfs(i,1,a[i]);
		b[i]=0;
	}
	for(int i=1;i<=cnt;i++)
	cout<<ans[i]<<' ';
	cout<<endl<<maxx;
	return 0;
}