#include <bits/stdc++.h>
using namespace std;

int main (){
    int N, M, K;
    cin >> N >> M >> K;
    map<int, int, greater<int>> cows;
    int weight, amount;
    for (int i = 0; i < N; ++i){
        cin >> weight >> amount;
        cows[weight] += amount;
    }
    queue<pair<int, int>> tower;
    tower.push({INT_MAX, M});
    long long result = 0, temp, count;
    for (auto &k : cows){
        count = 0;
        while (!tower.empty()){
            auto &p = tower.front();
            if (p.first >= k.first + K){
                temp = min(k.second, p.second);
                k.second -= temp;
                p.second -= temp;
                count += temp;
                if (p.second == 0){
                    tower.pop();
                }
                if (k.second == 0){
                    break;
                }
            }
            else{
                break;
            }
        }
        result += count;
        tower.push({k.first, count});
    }
    cout << result;
}