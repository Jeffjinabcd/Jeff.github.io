#include <bits/stdc++.h>
using namespace std;

int main() {
    int N;
    cin >> N;
    
    vector<pair<int, int>> cities(N);
    for (int i = 0; i < N; i++) {
        cin >> cities[i].first >> cities[i].second;
    }

    // 按照南岸城市坐标排序
    sort(cities.begin(), cities.end());

    // 提取北岸城市的坐标
    vector<int> north;
    for (int i = 0; i < N; i++) {
        north.push_back(cities[i].second);
    }

    // 寻找北岸城市的最长递增子序列（LIS）
    vector<int> lis;
    for (int i = 0; i < N; i++) {
        auto it = lower_bound(lis.begin(), lis.end(), north[i]);
        if (it == lis.end()) {
            lis.push_back(north[i]);
        } else {
            *it = north[i];
        }
    }

    // 输出最大可批准的航道数，即 LIS 的长度
    cout << lis.size() << endl;
    
    return 0;
}
