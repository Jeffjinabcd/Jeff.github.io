#include <bits/stdc++.h>
using namespace std;

const int MAX = 1e6 + 10;

int n, m;
int Log[MAX << 1], last[MAX << 1], f[MAX << 1][25], len[MAX << 1], st[MAX << 1];
// st[i]为第i个数为结尾的最长完美序列的起始位置
// len[i]为第i个数为结尾的最长完美序列的长度
// last[i]表示盈利值最近出现的位置
int findx(int left, int right) {
    if (st[left] == left) return left;

    if (st[right] < left) return right + 1;

    int l = left, r = right, ans = right + 1;

    while (l <= r) {
        int mid = (l + r) >> 1;

        if (st[mid] < left)
            l = mid + 1;
        else {
            ans = mid;
            r = mid - 1;
        }
    }

    return ans;
}

int query(int l, int r) {
    int s = Log[r - l + 1];
    return max(f[l][s], f[r - (1 << s) + 1][s]);
}

int main() {
    scanf("%d%d", &n, &m);
    Log[0] = -1;

    for (int i = 1; i <= n; i++) {
        int a;
        scanf("%d", &a);
        Log[i] = Log[i >> 1] + 1;
        st[i] = max(st[i - 1], last[a + MAX] + 1);  //数组下标不能为负数
        //第i个数为结尾的完美序列的起始位置必然在last[a[i]]之后
        len[i] = i - st[i] + 1;
        last[a + MAX] = i;
    }

    for (int i = 1; i <= n; i++) f[i][0] = len[i];

    for (int j = 1; (1 << j) <= n; j++) {
        for (int i = 1; i + (1 << j) - 1 <= n; i++) {
            f[i][j] = max(f[i][j - 1], f[i + (1 << (j - 1))][j - 1]);
        }
    }

    while (m--) {
        int l, r;
        cin >> l >> r;
        l++, r++;
        int mid = findx(l, r), ans = 0; // mid是右半部分的第一个位置

        if (mid > l) ans = mid - l;

        if (mid <= r) {
            ans = max(ans, query(mid, r));
        }

        printf("%d\n", ans);
    }

    return 0;
}