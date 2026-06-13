#include <bits/stdc++.h>
using namespace std;
#define ll long long

const int MAXN = 100010; // Maximum possible compressed height + 1
ll c[MAXN];     
ll a[MAXN];           // BIT array
int hair[MAXN];
int N;

int lowbit(int x) {
    return x & (-x);
}

void update(int x, int v) {
    for (int i = x; i < MAXN; i += lowbit(i))
        c[i] += v;
}

ll getSum(int x) {
    ll ans = 0;
    for (int i = x; i > 0; i -= lowbit(i))
        ans += c[i];
    return ans;
}

void update_a(int x, int v) {
    for (int i = x; i < MAXN; i += lowbit(i))
        a[i] += v;
}

ll getSum_a(int x) {
    ll ans = 0;
    for (int i = x; i > 0; i -= lowbit(i))
        ans += a[i];
    return ans;
}

int main () {
    freopen("haircut.in", "r", stdin);
    freopen("haircut.out", "w", stdout);
    cin >> N;
    for (int i = 0; i < N; ++i) {
        cin >> hair[i];
        ++hair[i];
    }
    for (int i = 0; i < N; ++i) {
        update(hair[i], 1);
        update_a(hair[i], getSum(N + 1) - getSum(hair[i]));
    }
    for (int i = 0; i < N; ++i) {
        cout << getSum_a(i) << endl;
    }
}