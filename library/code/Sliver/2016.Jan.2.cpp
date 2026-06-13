#include <bits/stdc++.h>
using namespace std;

struct se{
    int start = -1, end = -1;
};

int prefix[50002];

se result[7];

int main () {
    // freopen("testing.in", "r", stdin);
    freopen("div7.in", "r", stdin);
    freopen("div7.out", "w", stdout);
    int N, temp;
    cin >> N;
    for (int i = 1; i <= N; ++i){
        cin >> temp;
        prefix[i] = (prefix[i - 1] + temp) % 7;
        if (result[prefix[i]].start == -1){
            result[prefix[i]].start = i;
        }
        result[prefix[i]].end = i;
    }
    int res = 0;
    for (int i = 0; i < 7; ++i){
        res = max(res, result[i].end - result[i].start);
        // cout << result[i].end << "\t" << result[i].start << endl;
    }
    cout << res;
}