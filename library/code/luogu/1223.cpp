#include <bits/stdc++.h>
using namespace std;

struct node {
    int key, value;
};

struct cmp {
    bool operator()(node &a, node &b) {
        if (a.key == b.key){
            return a.value > b.value;
        }
        return a.key > b.key;
    }
};

priority_queue<node, vector<node>, cmp> pq;
int main () {
    // freopen("testing.in", "r", stdin);
    int N;
    cin >> N;
    for (int i = 0; i < N; ++i){
        int temp;
        cin >> temp;
        pq.push({temp, i + 1});
    }
    int result = 0, temp = 0;
    while (pq.size() > 0){
        cout << pq.top().value << " ";
        result += temp;
        temp += pq.top().key;
        // cout << result << endl;
        pq.pop();
    }
    cout << endl << fixed << setprecision(2) << (double) result / N;
}