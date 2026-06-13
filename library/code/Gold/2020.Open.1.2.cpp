#include <bits/stdc++.h>
#define ll long long
using namespace std;

int N;
vector<int> V[100001];
ll W[100001]; // W[i]��ʾ������������>i����������õ��������������������
int bit[100001];

int lowbit(int x) { return x & (-x); }

int getSum(int x) {
    int sum = 0;
    for (int i = x; i >= 1; i -= lowbit(i)) {
        sum += bit[i];
    }
    return sum;
}

void update(int x, int v) {
    for (int i = x; i <= N; i += lowbit(i)) {
        bit[i] += v;
    }
}

int main() {
    freopen("haircut.in", "r", stdin);
    freopen("haircut.out", "w", stdout);
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);

    cin >> N;
    for (int i = 1, x; i <= N; i++) {
        cin >> x;
        V[x].push_back(i);
    }

    ll ans = 0;
    for (int i = N; i >= 0; --i) {
        reverse (V[i].begin(), V[i].end());
        for (int v : V[i]) {
            ans += getSum(v);
            update(v, 1);
        }
        W[i] = ans;
    }
    for (int i = 0; i < N; ++i) {
        cout << W[0] - W[i] << endl;
    }

    return 0;
}
