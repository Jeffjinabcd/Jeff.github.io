#include <bits/stdc++.h>
using namespace std;

int n, to[200005], inDegree[200005], ans = INT_MAX;
bool vis[200005];

int circle(int p) {
    int res = 0;
    int temp = p;
    while (!vis[temp]){
        vis[temp] = true;
        temp = to[temp];
        ++res;
    }
    return res;
}

void del(int i)  //删除i
{
    if (vis[i]) {
        return;
    }
    --inDegree[to[i]];
    vis[i] = true;
    if (inDegree[to[i]] == 0){
        del(to[i]);
    }
    return;
}

/*
求图中最小的环。
如果一个人的入度为0，则肯定不可能成环，那么把这个人和他连接的边删去（即标记这个人并将他下一个人的入度减
1），
如果下一个人的入度为0则将他也删去，以此类推，最后把所有入度为0的人都删去了，剩下的都是环。
每个人的出度为1，所以每个人只能在一个环中，每条边只能在一个环中，也就是说每个环都是分开的，
然后进行一遍搜索，找出最小的环。
*/
int main() {
    // inDegree[i]为第i个人的入度
    // freopen("testing.in", "r", stdin);
    int i;
    cin >> n;
    for (i = 1; i <= n; i++) {
        scanf("%d", &to[i]);
        inDegree[to[i]]++;
    }

    for (i = 1; i <= n; i++) {
        if (inDegree[i] == 0 && !vis[i])
            del(i);  //如果 i 的入度为 0 且还未被删除，则删除i
    }

    for (i = 1; i <= n; i++) {
        if (!vis[i])  //如果i还未搜过且未被删除，则从i开始搜索
        {
            ans = min(ans, circle(i));
        }
    }

    printf("%d", ans);
    return 0;
}
