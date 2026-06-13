#include<bits/stdc++.h>
using namespace std;

int max_intercept_missiles(vector<int>& heights) {
    vector<int> lnis; // 最长不升子序列
    for (int h : heights) {
        auto pos = upper_bound(lnis.begin(), lnis.end(), h, greater<int>());
        if (pos == lnis.end()) {
            lnis.push_back(h);
        } else {
            *pos = h;
        }
    }
    return lnis.size();
}


int min_systems_needed(vector<int>& heights) {
    vector<int> lis; // 最长递增子序列（LIS 分解）
    for (int h : heights) {
        auto pos = lower_bound(lis.begin(), lis.end(), h);
        if (pos == lis.end()) {
            lis.push_back(h);
        } else {
            *pos = h;
        }
    }
    return lis.size();
}

int main() {
    vector<int> heights;
    int h;
    while (cin >> h) {
        heights.push_back(h);
    }
    cout << max_intercept_missiles(heights) << endl;
    cout << min_systems_needed(heights) << endl;
    return 0;
}