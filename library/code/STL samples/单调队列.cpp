#include <bits/stdc++.h>
using namespace std;

const int maxn=1e5+5;
deque<int> q_up; // 单调递增
deque<int> q_dw; // 单调递减
int a[maxn]={0,1,6,5,3,8};

int main(){
    for(int i=1;i<=5;i++){
        while(q_up.size() && q_up.back() >= a[i]) {
        	q_up.pop_back();
		}
        q_up.push_back(a[i]);
    }

    int n=q_up.size();
    for(int i=1;i<=n;i++) {
        printf("%d ", q_up.front());
        q_up.pop_front();
    }
    
    return 0;
}

