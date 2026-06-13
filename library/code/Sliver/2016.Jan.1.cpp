#include <bits/stdc++.h>
using namespace std;

int N, K;
set<int> record;

bool cows (int size){
    int ans = 1;
    auto p = record.begin();
    int left = *p;
    while (p != record.end()){
        ++p;
        if (*p > left + size * 2){
            ++ans;
            left = *p;
            // cout << left << endl;
        }
    }
    return ans <= K;
}

int main () {
    // freopen("testing.in", "r", stdin);
    freopen("angry.in", "r", stdin);
    freopen("angry.out", "w", stdout);
    cin >> N >> K;
    for (int i = 0; i < N; ++i){
        int temp;
        cin >> temp;
        record.insert(temp);
    }
    int left = 1, right = (*record.rbegin() - *record.begin()) / 2;
    while (left < right){
        int mid = left + (right - left) / 2;
        // cout << left << "\t" << mid << "\t" << right << endl;
        // cout << cows(mid) << endl;
        if (!cows(mid)){
            left = mid + 1;
        }
        else {
            right = mid;
        }
    }
    cout << left;
}