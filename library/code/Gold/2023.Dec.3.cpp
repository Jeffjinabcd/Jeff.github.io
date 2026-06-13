#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
int n;
ll A, B;
int x[200005];
ll prefix[200005], suffix[200005];  // ǰ׺�͡���׺��

ll check(ll y) {
    int p = upper_bound (x + 1, x + n + 1, y) - x - 1;
    return A * (p * y - prefix[p]) + B * (suffix[p + 1] - (n - p) * y);
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0), cout.tie(0);
    cin >> n;
    for (int i = 1; i <= n; i++) cin >> x[i];

    sort(x + 1, x + n + 1);
    for (int i = 1; i <= n; i++) prefix[i] = prefix[i - 1] + x[i];
    for (int i = n; i; i--) suffix[i] = suffix[i + 1] + x[i];
    
    // ���� int
    int t;
    cin >> t;
    while (t--) {
        cin >> A >> B;
        int l = 0, r = 1e6;
        while (r - l > 2) {
            int m1 = l + (r - l) / 3;
            int m2 = r - (r - l) / 3;
            if (check(m1) > check(m2)) {
                l = m1;
            }
            else {
                r = m2;
            }
        }
        
        ll ans = LLONG_MAX;
        for (int i = l; i <= r; i++) {
            ans = min(ans, check(i));
        }
        cout << ans << endl;
    }

    // ���� double
    // int t;
    // cin >> t;
    // while (t--) {
    //     cin >> A >> B;
    //     double l = 0, r = 1e6;
    //     while (r-l >=1e-6) {
    //          // ֱ������
    //         double m1 = l + (r - l) / 3;
    //         double m2 = r - (r - l) / 3;
    //         if(check(m1) > check(m2)) {
    //             l = m1;
    //         } else {
    //             r = m2;
    //         }
    //     }

    //     cout << min(check(l), check(r)) << "\n";
    // }
    return 0;
}