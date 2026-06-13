#include <iostream>
#include <vector>

using namespace std;

// 计算将 M 个苹果分成 N 个盘子的分法数
int appleDistribution(int M, int N) {
    // 创建一个 (M+1) x (N+1) 的二维表格
    vector<vector<int>> dp(M + 1, vector<int>(N + 1, 0));

    // 初始化表格
    for (int i = 0; i <= M; ++i) {
        dp[i][0] = 0; // 0 个盘子时，分法数为 0
        dp[i][1] = 1; // 1 个盘子时，只有一种分法
    }
    for (int j = 0; j <= N; ++j) {
        dp[0][j] = 1; // 0 个苹果时，无论多少盘子都只有一种分法
    }

    // 填充表格
    for (int i = 1; i <= M; ++i) {
        for (int j = 2; j <= N; ++j) {
            if (i < j) {
                dp[i][j] = dp[i][i]; // 苹果少于盘子数
            } else {
                dp[i][j] = dp[i][j - 1] + dp[i - j][j];
            }
        }
    }

    return dp[M][N];
}

int main() {
    int t; // 测试数据数量
    cin >> t;

    vector<int> results;
    while (t--) {
        int M, N;
        cin >> M >> N;
        results.push_back(appleDistribution(M, N));
    }

    // 输出结果
    for (int result : results) {
        cout << result << endl;
    }

    return 0;
}
