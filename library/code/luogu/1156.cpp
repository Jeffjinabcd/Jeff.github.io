#include<bits/stdc++.h>
using namespace std;

struct p{
    int t,f,h; // t表示垃圾被投进井中的时间； f表示该垃圾能维持卡门生命的时间； h该垃圾能垫高的高度。
} c[101];

int dp[101]; // dp[i]在高度为i的位置的最大生命值

bool cmp(const p &a, const p &b) {
    return a.t<b.t;
}

/*
这个题非常的像 01 背包，但是又有不同之处，因为 01 背包中的“不装”
就不对状态做修改，这里不装则是将垃圾堆起来。
*/
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    int d, g;  // d表示井的深度， g为被投入井的垃圾的数量
    cin>>d>>g;
    for(int i=1;i<=g;i++)
        cin>>c[i].t>>c[i].f>>c[i].h;

    sort(c+1, c+1+g, cmp);

    dp[0]=10;
    for(int i=1;i<=g;i++) // 垃圾数
        for(int j=d;j>=0;j--) { // 高度
            if(dp[j]>=c[i].t){
                if(j+c[i].h>=d){
                    cout<<c[i].t; // 最早爬出陷阱的时间
                    return 0;
                }
                // 注意顺序，先做不吃，再做吃，不然dp[j]会被更新 
                dp[j+c[i].h]=max(dp[j+c[i].h], dp[j]); // 不吃垃圾用它来堆，此时高度+=这个垃圾的高度
                dp[j]+=c[i].f; // 吃垃圾，此时高度不变, 生命值增加
            }
        }

    cout<<dp[0]; // 出不去存活的最长时间
    return 0;
}
