#include <bits/stdc++.h>
using namespace std;

// ???????????
// pq.push(n);
// pq.pop();
// pq.top();
struct my_priority_queue {
    vector<int> data;

     my_priority_queue() { // ??????
         data.resize(1, 0);
     }

    void push(int n) {
        data.push_back(n);
        moveUp(size());
    }

    void pop() {
        data[1] = data[size()];
        data.pop_back();
        moveDown(1);
    }

    int top() {
        return data[1];
    }

    int size() {
        return data.size() - 1;
    }

    bool empty() {
        return size() == 0;
    }
 
    void moveUp(int i) { // i??????
        if (size() == 1) return;

        while (i != 1) {
            if (data[i] < data[i/2]) {
                swap(data[i], data[i/2]);
                i = i/2;
            }
            else {
                return;
            }
        }
        return;

    }

    void moveDown(int i) { // i??????
        if (size() == 1) return;
        int t = i;

        while (t*2 <= size()) {
            if (data[t] > data[i*2]) {
                t = t * 2;
            }
            if (i*2 + 1 <= size()) {
                if (data[i*2 + 1] < data[t]) {
                    t = i * 2 + 1;
                }
            }
            if (t != i) {
                swap(data[t], data[i]);
                i = t;
            }
            else {
                return;
            }
        }
    }
};

int main() {
    my_priority_queue pq;
    
    // int n, t;
    // cin>>n;

    // for(int i=0; i<n; i++) {
    //     cin>>t;
    //     pq.push(t);
    // }

    // while (!pq.empty()) {
    //     cout<<pq.top()<<" ";
    //     pq.pop();
    // }

    // P1090
    // int n;
    // int p,q;
    // cin>>n;
    // for(int i=0; i<n; i++)
    // {
    //     scanf("%d", &p);
    //     pq.push(p);
    // }
    // int sum=0;
    // for(int i=0;i<n-1;i++)
    // {
    //     p=pq.top();
    //     pq.pop();
    //     q=pq.top();
    //     pq.pop();
	//     sum+=p+q;
    //     pq.push(p+q);
    // }
    // cout<<sum<<endl;


    return 0;
}
