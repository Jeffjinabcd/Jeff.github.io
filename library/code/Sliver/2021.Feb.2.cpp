#include <bits/stdc++.h>
using namespace std;

int main (){
    //freopen("testing.in", "r", stdin);
    int N, K, num;
    cin >> N >> K;
    set<int> years;
    for (int i = 0; i < N; ++i){
        cin >> num;
        years.insert(num);
    }
    int max = *years.rbegin() / 12;
    if (*years.rbegin() % 12 != 0){
        ++max;
    }
    auto j = years.begin();
    int temp = 0;
    vector<int> gaps(K);
    for (int i = 12; i <= max*12 && j != years.end(); i += 12){
        if (*j < i){
            gaps.push_back(temp);
            temp = 0;
            while (*j < i && j != years.end()){
                ++j;
            }
        }
        else{
            ++temp;
        }
    }
    sort(gaps.begin(), gaps.end());
    for (int i = gaps.size() - 1; i > gaps.size() - K; --i){
        max -= gaps[i];
    }
    cout << max*12;
}