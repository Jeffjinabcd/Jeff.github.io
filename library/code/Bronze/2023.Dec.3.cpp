#include <bits/stdc++.h>
using namespace std;

struct plant{
    int h;
    int a;
    int r;
};

bool rule(const plant& x, const plant& y){
    return x.r < y.r;
}

int main () {
    //freopen("testing.in", "r", stdin);
    int T;
    cin >> T;
    for (int p = 0; p < T; ++p){
        int N;
        cin >> N;
        vector<plant> all(N);
        for (int i = 0; i < N; ++i){
            cin >> all[i].h;
        }
        for (int i = 0; i < N; ++i){
            cin >> all[i].a;
        }
        for (int i = 0; i < N; ++i){
            cin >> all[i].r;
        }
        sort(all.begin(), all.end(), rule);
        long long low = 0, high = 9223372036854775807, temp;
        for (int i = 0; i < N - 1 && low <= high; ++i){
            if (all[i].h > all[i + 1].h && all[i].a < all[i + 1].a){
                temp = (all[i].h - all[i + 1].h) / (all[i + 1].a - all[i].a);
                if ((all[i].h - all[i + 1].h) % (all[i + 1].a - all[i].a) == 0){
                    --temp;
                }
                high = min(high, temp);
                //cout << "high:" << high << endl;
            }
            else if (all[i].h < all[i + 1].h && all[i].a > all[i + 1].a){
                temp = (all[i + 1].h - all[i].h) / (all[i].a - all[i + 1].a) + 1;
                low = max(low, temp);
                //cout << "low:" << low << endl;
            }
            else if (all[i].h > all[i + 1].h && all[i].a >= all[i + 1].a){
                continue;
            }
            else{
                low = 9223372036854775807;
                high = 0;
            }
        }
        if (low <= high){
            cout << low << endl;
        }
        else{
            cout << -1 << endl;
        }
    }
}