#include <bits/stdc++.h>
using namespace std;

priority_queue<int, vector<int>, greater<int> > pq;  // 小根堆
priority_queue<int>  pq;   // 默认大根堆

// 常见操作
// pq.push(n);
// pq.pop();
// pq.top();
// pq.size()
// pq.empty()

int main()
{
    int n;
    int p,q;
    cin>>n;
    for(int i=0; i<n; i++)
    {
        scanf("%d", &p);
        pq.push(p);
    }
    int sum=0;
    for(int i=0;i<n-1;i++)
    {
        p=pq.top();
        pq.pop();
        q=pq.top();
        pq.pop();
	    sum+=p+q;
        pq.push(p+q);
    }

    cout<<sum<<endl;

    return 0;
}
