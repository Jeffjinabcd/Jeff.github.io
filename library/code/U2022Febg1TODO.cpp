#include <bits/stdc++.h>
#define ll long long
using namespace std;

int N, Q, E[18];
ll f[1 << 18][18]; // f[S][v] 还剩点集s中的点未被选择过，已经选择的部分为若干个已经完成的环，
                   // 与一条起点为u (=max(S))终点为v的路径，剩下的点有多少种可能的连接方法。
ll g[1 << 18];    //g[S]表示有多少种方式能够将点集S内的点连成一张由若干个互不相交的环形成的有向图

int main() {
    ios::sync_with_stdio(false);
	cin.tie(0), cout.tie(0);
    cin >> N;

    for (int i = 0; i < N; i++) {
        int a[18];
        for (int j = 0; j < N; j++) {
            cin >> a[j];
            a[j]--;
        }
        int L = N;
        while (a[L - 1] != i) L--; // 愿望单前L个礼物可以换
        for (int j = 0; j < L; j++) E[i] |= 1 << a[j]; // 记录礼物i可以换哪些礼物
    }

    g[0] = 1;
    for (int max = 0; max < N; max++) { // 令max作为路径的起点，并且是集合中最大的点
       // TODO
    }

    cin >> Q;
    for (int i = 0; i < Q; i++) {
        char s[19];
        cin >> s;
        int mask1 = 0, mask2 = 0;
        // TODO
        cout << g[mask1] * g[mask2] << endl;
        // note: if mask is 010011, (1 << N) - 1 - mask is 101100
    }

    return 0;
}
