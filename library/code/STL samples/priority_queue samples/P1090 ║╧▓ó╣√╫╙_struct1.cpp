#include <bits/stdc++.h>
using namespace std;
struct node
{
    int m;
    bool operator<(const node &a) const //运算符重载, 优先队列，重量小的在前
    {
        return m > a.m;
    }
};

int main()
{
    int n;
    priority_queue<node> pq;
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
