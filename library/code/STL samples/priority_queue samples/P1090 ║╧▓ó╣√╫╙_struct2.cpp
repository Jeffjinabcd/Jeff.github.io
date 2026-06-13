#include <bits/stdc++.h>
using namespace std;
struct node
{
    int m;
};
struct cmp {     // 仿函数
    bool operator()(node &a, node &b) {
        return a.m > b.m;
    }
};

priority_queue<node, vector<node>, cmp> pq;

int main()
{
    int n;
    node p, q;
    cin>>n;
    for(int i=0; i<n; i++)
    {
        scanf("%d", &p.m);
        pq.push(p);
    }
    int sum=0;
    for(int i=0;i<n-1;i++)//n个果子只需要n-1次合并
    {
        p=pq.top();
        pq.pop();
        q=pq.top();
        pq.pop();

        sum+=p.m+q.m;

        p.m=p.m+q.m;
        pq.push(p);
    }

    cout<<sum<<endl;
    return 0;
}
