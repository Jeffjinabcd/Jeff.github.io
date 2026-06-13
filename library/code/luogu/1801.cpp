#include <bits/stdc++.h>
using namespace std;

int add[200001];

priority_queue<int> pq1; // 大根堆  放排序后的前i-1个数 
priority_queue<int, vector<int>, greater<int> > pq2; // 小根堆 放排序后的i~n个数

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	
    int m, n;
	cin>>m>>n;
	for(int i=1; i<=m; i++) {
		cin>>add[i];
	}

	int pos = 1;
	for(int i=1; i<=n; i++) {
		int q;
		cin>>q;
		for(int j=pos; j<=q; j++) {
			pq1.push(add[j]);
			if(pq1.size() == i) {  // 第i大的数应该放入pq2
				pq2.push(pq1.top());
				pq1.pop();
			}
		}
		cout<<pq2.top()<<endl;    // 输出每次get的答案

		pos=q+1; // 下一个将要装入的数据位置
		pq1.push(pq2.top());
		pq2.pop();
	}
	
	return 0;
} 
