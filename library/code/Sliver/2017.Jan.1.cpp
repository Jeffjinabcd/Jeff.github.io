#include <bits/stdc++.h>
using namespace std;

int N, T;
int cows[10001];

bool check (int mid){
    priority_queue<int, vector<int>, greater<int> > pq;
    int i = 0;
    while (pq.size() < mid){
        pq.push(cows[i]);
        ++i;
    }
    for (int j = i; j < N; ++j){
        pq.push(pq.top() + cows[j]);
        pq.pop();
    }
    while (pq.size() > 1){
        pq.pop();
    }
    return pq.top() <= T;
}

int main (){
    // freopen("testing.in", "r", stdin);
    freopen("cowdance.in", "r", stdin);
    freopen("cowdance.out", "w", stdout);
    cin >> N >> T;
    for (int i = 0; i < N; ++i){
        cin >> cows[i];
    }
    int left = 1, right = N;
    int ans = 0;
    while (left <= right){
        int mid = (left + right)/2;
        if (check(mid)){
            right = mid - 1;
            ans = mid;
        }
        else{
            left = mid + 1;
        }
    }
    cout << ans;
}