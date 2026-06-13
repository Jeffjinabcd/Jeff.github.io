#include <bits/stdc++.h>
using namespace std;

const int N = 5e4 + 5;

int n;
int sum[N];
int maxd[N], secd[N];

void dp() {
    for (int v = n; v >= 1; --v) {
        if (sum[v] < v) {
            int u = sum[v];
            if (maxd[v] + 1 > maxd[u]) {
                secd[u] = maxd[u], maxd[u] = maxd[v] + 1;
            }
            else if (maxd[v] + 1 > secd[u]) {
                secd[u] = maxd[v] + 1;
            }
        }
    }
}

int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i)
        for (int j = i + i; j <= n; j += i)
            sum[j] += i;  // 预处理每个数的约数和
    dp();
    int ans = 0;
    for (int i = 1; i <= n; ++i) ans = max(ans, maxd[i] + secd[i]);
    printf("%d\n", ans);
    return 0;
}
