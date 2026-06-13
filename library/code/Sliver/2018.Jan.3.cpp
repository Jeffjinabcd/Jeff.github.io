#include <bits/stdc++.h>
using namespace std;

struct edge {
    int end, w;
};

vector<edge> record[5001];

bool vis[5001];

int main (){
    // freopen("testing.in", "r", stdin);
    freopen("mootube.in", "r", stdin);
    freopen("mootube.out    ", "w", stdout);
    int N, Q;
    cin >> N >> Q;
    for (int i = 0; i < N - 1; ++i){
        int p, q, r;
        cin >> p >> q >> r;
        record[p].push_back({q, r});
        record[q].push_back({p, r});
    }
    for (int i = 0; i < Q; ++i){
        int k, v;
        cin >> k >> v;
        int result = 0;
        queue<int> record1;
        record1.push(v);
        vis[v] = true;
        while (!record1.empty()){
            int v1 = record1.front();
            record1.pop();
            for (auto &e : record[v1]){
                // cout << e.end << "\t" << e.w << "\t" << k << vis[e.end];
                if (!vis[e.end] && e.w >= k){
                    // cout << "\ttruetrue";
                    ++result;
                    record1.push(e.end);
                    vis[e.end] = true;
                }
                // cout << endl;
            }
        }
        memset(vis, 0, sizeof(vis));
        cout << result << endl;
    }
}