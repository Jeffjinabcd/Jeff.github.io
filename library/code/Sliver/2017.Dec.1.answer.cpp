#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1e5 + 1;
int score[MAXN], prefix[MAXN], lowest[MAXN];
double avg[MAXN];

int main() {
    // freopen("homework.in","r",stdin);
    // freopen("homework.out","w",stdout);
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    int n;
    cin >> n;
    for(int i = 1; i <= n; i++) {
        cin >> score[i];
        prefix[i] = prefix[i-1] + score[i];
    }

    lowest[n] = score[n];
    for(int i = n - 1; i >= 1; i--) {
        lowest[i] = min(lowest[i+1], score[i]);
    }

    double maxAvg = 0;
    for(int k=1; k<=n-2; k++) {
        avg[k] = 1.0 * (prefix[n] - prefix[k] - lowest[k + 1]) / (n - k -1);
        maxAvg = max(maxAvg, avg[k]);
    }

    for(int i = 1; i <= n-2; i++) {
        if(maxAvg == avg[i]) {
            cout << i << endl;
        }
    }

    return 0;
}
