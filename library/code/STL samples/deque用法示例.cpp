#include<bits/stdc++.h>
//#include<queue>
using namespace std;
const int maxn=1e5+5;
deque<int> dq;
int a[maxn]={0,1,2,3,4,5,6,7,8,9,10};

int main(){
    dq.push_front(a[1]);
	dq.push_front(a[2]);
	dq.push_front(a[3]);
    
    dq.push_back(a[4]);
	dq.push_back(a[5]);
	dq.push_back(a[6]);
    
    dq.push_front(a[7]);
	dq.push_front(a[8]);
    
    dq.push_back(a[9]);
	dq.push_back(a[10]);
    
    dq.pop_front();
	dq.pop_front();
    
    dq.pop_back();
	dq.pop_back();
	
//    dq.empty(); 判断队列是否为空 
    int n=dq.size(); //n=6
    for(int i=1;i<=n;i++){
        printf("%d ", dq.front());
        dq.pop_front();
    }
    return 0;
}
// 输出：3
