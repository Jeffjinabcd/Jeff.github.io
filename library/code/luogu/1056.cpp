#include <bits/stdc++.h>

using namespace std;
struct node {
    int key, value;
};

struct cmp {
    bool operator()(node &a, node &b) {
        return a.value < b.value;
    }
};

priority_queue<node, vector<node>, cmp> krec;
priority_queue<node, vector<node>, cmp> lrec;

int main () {
    // freopen("testing.in", "r", stdin);
    int M, N, K, L, D;
    cin >> M >> N >> K >> L >> D;
    map<int, int> Krec;
    map<int, int> Lrec;
    for (int i = 0; i < D; ++i){
        int temp1, temp2, temp3, temp4;
        cin >> temp1 >> temp2 >> temp3 >> temp4;
        if (temp1 == temp3){
            ++Lrec[min(temp2, temp4)];
        }
        else if (temp2 == temp4){
            ++Krec[min(temp1, temp3)];
        }
    }
    for (auto &k : Krec){
        krec.push({k.first, k.second});
    }
    
    for (auto &k : Lrec){
        lrec.push({k.first, k.second});
    }
    set<int> result;
    for (int i = 0; i < K; ++i){
        result.insert(krec.top().key);
        krec.pop();
    }
    for (auto &k : result){
        cout << k << " ";
    }
    cout << endl;
    set<int> result1;
    for (int i = 0; i < L; ++i){
        result1.insert(lrec.top().key);
        lrec.pop();
    }
    for (auto &k : result1){
        cout << k << " ";
    }
}