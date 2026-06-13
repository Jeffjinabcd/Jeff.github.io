#include <bits/stdc++.h>
using namespace std;

const int MAX_N = 101;
const int INF = 0x3f3f3f3f;
int f1[MAX_N * 2][MAX_N * 2],
    f2[MAX_N * 2][MAX_N * 2];  // f[i][j]表示把第i堆到第j堆的石头合并成一堆的最优值
int num[MAX_N * 2], sum[MAX_N * 2];  // sum[i]表示前i堆石头的价值总和（前缀和）

/*
请注意题干要求(要和一本通例题区分开来)： 圆形操场, 要P1880
区间DP
*/
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &num[i]);
        num[i + n] = num[i];
    }

    for (int i = 1; i <= 2 * n; ++i) {  // 求num前缀和
        sum[i] = sum[i - 1] + num[i];
    }

    int min_ans = INF, max_ans = 0;
    for (int i = 2 * n - 1; i > 0; --i)
        for (int j = i + 1; j - i < n && j <= 2 * n; ++j) {
            f1[i][j] = INF;
            for (int k = i; k < j; ++k) {
                f1[i][j] = min(f1[i][j], f1[i][k] + f1[k + 1][j] + sum[j] - sum[i - 1]);
                f2[i][j] = max(f2[i][j], f2[i][k] + f2[k + 1][j] + sum[j] - sum[i - 1]);
            }
        }

    for (int i = 1; i <= n; ++i) {
        min_ans = min(min_ans, f1[i][i + n - 1]);
        max_ans = max(max_ans, f2[i][i + n - 1]);
    }

    printf("%d\n%d\n", min_ans, max_ans);
    return 0;
}
