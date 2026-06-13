#include <bits/stdc++.h>
using namespace std;

int main (){
    // freopen("testing.in", "r", stdin);
    priority_queue<int, vector<int>, greater<int>> pq;
    int n, m;
    cin >> n >> m;
    queue<int> record;
    for (int i = 0; i < n; ++i){
        int temp;
        cin >> temp;
        record.push(temp);
    }
    while (pq.size() < m){
        pq.push(record.front());
        record.pop();
    }
    while (record.size() > 0){
        pq.push(pq.top() + record.front());
        pq.pop();
        record.pop();
    }
    while (pq.size() > 1){
        pq.pop();
    }
    cout << pq.top() << endl;
}