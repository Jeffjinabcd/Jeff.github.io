#include <bits/stdc++.h>
using namespace std;

struct cow {
    int pos, id;
};

int N, ans = INT_MAX, sss;
cow cows[50005];
map<int, int> ids;

bool cmp(const cow &a, const cow &b){
    return a.pos < b.pos;
}

int main () {
    // freopen("testing.in", "r", stdin);
    // freopen("lineup.in", "r", stdin);
    // freopen("lineup.out", "w", stdout);
    cin >> N;
    for (int i = 0; i < N; ++i){
        cow temp;
        cin >> temp.pos >> temp.id;
        cows[i] = temp;
        ids[temp.id]++;
    }
    sort(cows, cows + N, cmp);
    sss = ids.size();
    ids.clear();
    int l = 0;
    for (int r = 0; r < N; ++r){
        ids[cows[r].id]++;
        while (ids[cows[l].id] > 1){
            ids[cows[l].id]--;
            ++l;
        }
        if (ids.size() == sss){
            // cout << cows[r].pos << " " << cows[l].pos << endl;
            ans = min (ans, cows[r].pos - cows[l].pos);
        }
    }
    cout << ans;
}