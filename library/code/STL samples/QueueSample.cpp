#include <bits/stdc++.h>
//#include <queue>
using namespace std;

queue<int> q;

// q.push(n)  添加n到队尾 
// q.pop()    删除队首元素 
// q.front()  查看队首元素
// q.back()   查看队尾元素
// q.size()   查看队列中元素个数
// q.empty()  判断队列是否为空 

int main() {
	int n;
	cin>>n;
	for(int i=1; i<=n; i++) {
		 int t;
		 cin>>t;
		 q.push(t);
	}

	while(!q.empty()) {
		cout<<q.front()<<" ";
		q.pop();
	}

    return 0;
}
