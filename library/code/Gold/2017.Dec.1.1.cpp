#include <bits/stdc++.h>
using namespace std;
long long n, d;
struct pie {
    int b, e, id;
} pieB[100005], pieE[100005];

bool cmpB(const pie &a, const pie &b) { return a.b < b.b; }

bool cmpE(const pie &a, const pie &b) { return a.e < b.e; }

struct boundB {
    bool operator()(const pie &a, const pie &b) { 
        return a.b < b.b;
    }
};

struct boundE {
    bool operator()(const pie &a, const pie &b) {
        return a.e < b.e;
    }
};

int dis[200005];
vector<int> g[200005];
queue<int> q;
int main() {
    //	freopen("piepie.in", "r", stdin);
    //  freopen("piepie.out", "w", stdout);
    ios::sync_with_stdio(0);
    cin.tie(0);
    memset(dis, -1, sizeof(dis));  // 只能初始化成0或者-1
    cin >> n >> d;
    for (int i = 1; i <= n; i++) {
        cin >> pieB[i].b >> pieB[i].e;
        pieB[i].id = i;
        if (pieB[i].e == 0)  //只有对方美味值为0的点才是起始点
        {
            dis[i] = 1;
            q.push(i);
        }
    }
    for (int i = 1; i <= n; i++) {
        cin >> pieE[i].b >> pieE[i].e;
        pieE[i].id = i + n;
        if (pieE[i].b == 0)  //只有对方美味值为0的点才是起始点
        {
            dis[i + n] = 1;
            q.push(i + n);
        }
    }
    sort(pieB + 1, pieB + n + 1, cmpB);
    sort(pieE + 1, pieE + n + 1, cmpE);

    //建图：
    for (int i = 1; i <= n; i++)  // E给了B一个派（pieE[i]），查一查B可以回赠哪些派？
    {
        int pos = lower_bound(pieB + 1, pieB + n + 1, pieE[i], boundB()) - pieB;  // 求出大于等于pieE[i]的最小值位置
        for (int j = pos; j <= n; j++) {
            if (pieB[j].b > pieE[i].b + d) {
                break;
            }
            g[pieB[j].id].push_back(pieE[i].id);  //倒过来连边，为了从结果向起点BFS
        }
    }

    for (int i = 1; i <= n; i++) {
        // 求出大于等于pieB[i]的最小值位置
        int pos = lower_bound(pieE + 1, pieE + n + 1, pieB[i], boundE()) - pieE;
        for (int j = pos; j <= n; j++) {
            if (pieE[j].e > pieB[i].e + d) break;
            g[pieE[j].id].push_back(pieB[i].id);
        }
    } 

    // bfs
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : g[u]) {
            if (dis[v] != -1) continue;  //已访问过
            dis[v] = dis[u] + 1;
            q.push(v);
        }
    }

    for (int i = 1; i <= n; i++) cout << dis[i] << endl;

    return 0;
}
