#include <bits/stdc++.h>
using namespace std;

int N, K, ans = 0;
map<int, int> ids;
int n[100010];

int main () {
    // freopen("testing.in", "r", stdin);
    freopen("lineup.in", "r", stdin);
    freopen("lineup.out", "w", stdout);
    cin >> N >> K;
    for (int i = 0; i < N; ++i){
        cin >> n[i];
    }
    int l = 0;
    for (int r = 0; r < N; ++r){
        ids[n[r]]++;
        while (ids.size() > K + 1 && l <= r){
            ids[n[l]]--;
            if (ids[n[l]] == 0){
                ids.erase(n[l]);
            }
            ++l;
        }
        if (ids[n[r]] > ans && ids.size() <= K + 1){
            ans = max(ans, ids[n[r]]);
        }
    }
    cout << ans;
}