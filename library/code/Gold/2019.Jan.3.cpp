#include <bits/stdc++.h>
using namespace std;

#define MAXN 10005
#define LL long long

struct Point {
    int v;        // ������
    LL w;  // ��ǰ����
};

struct edge {
    int to;  // ���ڵĵ�
    int w;   // ͨ����ʱ��
};

struct cmp {
    bool operator()(Point p1, Point p2) { return p1.w > p2.w; }
};

int c[MAXN];    // ÿ�����ʼţ��ͷ��
int par[MAXN];  // ÿ����ĸ��ڵ�
LL dist[MAXN];  // ������1�����·��
LL nums[MAXN];  // �����õ��ţ������
bool book[MAXN];

// ԭʼͼ
vector<edge> graph[MAXN];

//ʹ���ڽӱ���ʾ���·����
vector<int> sptree[MAXN];

//��һ��DFS, ��ÿ�������ܹ��ᾭ������ͷţ�����
void dfs(int cur, int fa) {
    nums[cur] = c[cur];
    for (int to : sptree[cur]) {
        if (to == fa) {
            continue;
        }
        dfs(to, cur);
        nums[cur] += nums[to];
    }
}

void dijkstra() {
    memset(dist, 0x7f, sizeof(dist));
    dist[1] = 0;
    priority_queue<Point, vector<Point>, cmp> pq;
    pq.push({1, 0});

    while (!pq.empty()) {
        Point cur = pq.top();
        pq.pop();
        int u = cur.v;
        if (book[u]) continue;
        book[u] = true;
        for (edge e : graph[u]) {
            int v = e.to;
            LL newdis = dist[u] + e.w;
            if (newdis < dist[v]) {
                dist[v] = newdis;
                par[v] = u;
                pq.push({v, newdis});
            } else if (newdis == dist[v]) {
                if (u < par[v]) {
                    par[v] = u;
                }
            }
        }
    }
}

int main() {
    freopen("shortcut.in", "r", stdin);
    freopen("shortcut.out", "w", stdout);
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, m, t;
    cin >> n >> m >> t;

    for (int i = 1; i <= n; i++) {
        cin >> c[i];
    }

    for (int i = 1; i <= m; i++) {
        int a, b, w;
        cin >> a >> b >> w;
        graph[a].push_back({b, w});
        graph[b].push_back({a, w});
    }

    // ��һ��Dijkstra�㷨�����·
    dijkstra();

    // �������·����
    for (int i = 2; i <= n; i++) {
        sptree[i].push_back(par[i]);
        sptree[par[i]].push_back(i);
    }

    // ��һ��dfs���ÿ������м�ͷ��ţ����
    dfs(1, -1);

    LL answer = 0;
    for (int i = 1; i <= n; i++) {
        //��ʡ��ʱ��Ϊnums[i] * (djik[i] - t)
        answer = max(answer, nums[i] * (dist[i] - t));
    }

    cout << answer << endl;

    return 0;
}
