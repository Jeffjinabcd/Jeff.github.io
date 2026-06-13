#include<bits/stdc++.h>
#define ll long long
#define INF 0x3f3f3f3f3f3f3f3f
using namespace std;

int n,m,minn,t[510];

ll f[510][210], ans; // f[i][j]表示前i个人的最小等车时间总和, j表示第i个人等车的时间。

/*
由题可知，若车在人大附中停了m分钟，那么不如在到人大附中时先送走一批人（即便当时没人），所以停车时间T1<m
设一个人等到车从人民大学回来的时间为T2, 因为车往返一次只需m分钟，所以T2<m
综上，一个人等车的时间 j=T1+T2<2m
*/
int main()
{
	// freopen("bus.in","r",stdin);
    // freopen("bus.out","w",stdout);

    scanf("%d%d",&n,&m);
    for(int i = 1;i <= n; ++i) {
    	scanf("%d", t + i);
		minn = min(minn, t[i]);
    }

    for(int i = 1; i <= n; ++ i) t[i] -= minn;

    sort(t + 1, t + n + 1);
    memset(f, 0x3f, sizeof(f));
    for(int i = 0;i < 2 * m; ++i) f[1][i] = i;
    
    for(int i = 2; i <= n; ++i)
        for(int j = 0; j < 2 * m; ++j) {  // 上一个人的等待时间
            if(f[i-1][j] < INF) { // 排除无效状态
                // 若第i个人能赶上第i-1个人所在车次
                if(t[i-1] + j >= t[i]) {
                    int waitT = t[i-1] + j - t[i];
                    f[i][waitT] = min(f[i][waitT], f[i-1][j] + waitT);
                }
                else if (t[i-1] + j + m >= t[i]) {
                    // 若第i个人没赶上第i-1个人所在车次，在车回来之前开始等车
                    for (int k=0; t[i-1] + j + m + k - t[i] < m * 2; ++k) { // 枚举车回来后停的时间k
                        int waitT =t[i-1] + j + m + k - t[i];
                        f[i][waitT] = min(f[i][waitT], f[i - 1][j] + waitT);
                    }
                }
                else if(t[i-1] + j + m < t[i])  {
                    // 若第i个人没赶上第i-1个人所在车次，在车回来之后才到
                    for (int k = 0; k < m; ++k) {// 枚举第i人等待的时间k
                        int waitT = k;
                        f[i][waitT] = min(f[i][waitT], f[i - 1][j] + waitT);
                    }
                }
            }
        }

    ans = INF;
    for(int i = 0;i < m * 2; ++i)
        ans = min(ans, f[n][i]);
    cout<<ans<<endl;

    return 0;
}
