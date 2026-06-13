#include<bits/stdc++.h>
using namespace std;

int n,m,t[505],dp[505][201];  // dp[i][j]  从第i个人开始，且第i个人等了j分钟，总共需要的最小等待时间

/*
    由题可知，若车在人大附中停了m分钟，那么不如在到人大附中时先送走一批人（即便当时没人），所以停车时间T1<m
    设一个人等到车从人民大学回来的时间为T2, 因为车往返一次只需m分钟，所以T2<m
    综上，一个人等车的时间 j=T1+T2<2m
*/
int search(int i, int st) // i：第i个人，st：第i个人所在的车最早出发时间st, 返回从第i个人开始一共需要的等待时间
{
    if (i==n+1)//所有人都上车了 
        return 0;
    if (st<t[i])//如果现在的时间没有人，就到下一个人的到达时间 
        return search(i,t[i]);
    if (dp[i][st-t[i]]) //记忆化
        return dp[i][st-t[i]];

    int sum=0, j=i;
    // 在第i个人最早开车时间出发
    while (j<=n && t[j]<=st) {
    	sum += t[j];
    	j++;
	}
    int waitT = st * (j - i) - sum;
    int ans = waitT + search (j, st + m);
    for (; j <= n; ++j) {
        sum += t[j];
        waitT = t[j] * (j - i + 1) - sum;
        ans = min(ans, waitT + search(j + 1, t[j] + m));
    }
    dp[i][st - t[i]] = ans;
    return ans;
}

int main()
{
//	freopen("bus.in","r",stdin);
//	freopen("bus.out","w",stdout);

	scanf("%d%d",&n,&m);
	for(int i=1;i<=n;++i)
		scanf("%d", t+i);
    sort(t+1,t+n+1);//从小到大按照时间排序
    cout << search(1, 0) << endl;
    return 0;
}
