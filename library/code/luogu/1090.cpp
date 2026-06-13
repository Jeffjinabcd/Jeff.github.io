#include <bits/stdc++.h>
using namespace std;

int main () {
    // freopen("testing.in", "r", stdin);
    priority_queue<int, vector<int>, greater<int>> pq;
    int N;
    cin >> N;
    int temp;
    for (int i = 0; i < N; ++i){
        cin >> temp;
        pq.push(temp);
    }
    int result = 0;
    while (pq.size() > 1){
        temp = pq.top();
        result += temp;
        pq.pop();
        result += pq.top();
        pq.push(pq.top() + temp);
        pq.pop();
    }
    cout << result;
}