#include <bits/stdc++.h>
//#include <stack>
using namespace std;

stack<int> st;

// st.push(n)  添加n到栈顶
// st.pop()    删除栈顶元素
// st.top()    查看栈顶元素
// st.size()   查看栈中元素个数
// st.empty()  判断栈是否为空

int main() {
	int n;
	cin>>n;
	for(int i=1; i<=n; i++) {
		 int t;
		 cin>>t;
		 st.push(t);
	}

	while(!st.empty()) {
		cout<<st.top()<<" ";
		st.pop(); 
	}

    return 0;
}
