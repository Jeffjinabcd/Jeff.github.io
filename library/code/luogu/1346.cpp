#include <bits/stdc++.h>
using namespace std;

int N, A, B;
int mapp[101][101];

int main () {
    cin >> N >> A >> B;
    for (int i = 1; i <= N; ++i) {
        int temp;
        cin >> temp;
        for (int j = 1; j <= N; ++j) {
            mapp[i][j] = INT_MAX;
        }
        int uni;
        cin >> uni;
        mapp[i][uni] = 0;
        for (int j = 1; j < temp; ++j) {
            int num;
            cin >> num;
            mapp[i][num] = 1;
        }
        mapp[i][i] = 0;
    }
    for (int k = 1; k <= N; ++k) {
        for (int j = 1; j <= N; ++j) {
            for (int i = 1; i <= N; ++i) {
                mapp[j][i] = min(mapp[j][i], mapp[k][i] + mapp[j][k]);
            }
        }
    }
    if (mapp[A][B] == INT_MAX){
        cout << -1;
    }
    else {
        cout << mapp[A][B];
    }
}