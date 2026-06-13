#include <bits/stdc++.h>
#define maxn 100001
using namespace std;
int n, a[maxn], c[maxn], ans[maxn];

int lowbit(int x) { return x & (-x); }

void update(int x, int k) {
    for (int i = x; i <= n; i += lowbit(i)) {
        c[i] += k;
    }
}

int query(int x) {
    int ans = 0;
    for (int i = x; i; i -= lowbit(i)) {
        ans += c[i];
    }
    return ans;
}

int main() {
    freopen("sleepy.in", "r", stdin);
    freopen("sleepy.out", "w", stdout);
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a[i]);
    }

    int sum = n;  // sum��ʾĿǰ��Ҫ�ƶ��ĸ���
    int cur = n + 1;
    for (int i = n; i > 0 && cur > a[i]; --i) {
        sum--;
        cur = a[i];
        update(a[i], 1);
    }

    for (int i = 1; i <= sum; ++i){
        ans[i] = sum - i + query(a[i] - 1);
        update(a[i], 1);
    }

    printf("%d\n", sum);
    for (int i = 1; i < sum; i++) {
        printf("%d ", ans[i]);
    }
    printf("%d", ans[sum]);
    return 0;
}