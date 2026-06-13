#include <bits/stdc++.h>
using namespace std;
int main()
{
    int n, m, now = 1;
    queue<int> q;
    cin >> n >> m;
    for (int i = 1; i <= n; i++){
        q.push(i);
    }
    while (!q.empty())
    {
        if (now == m){
            now = 1;
            cout << q.front() << " ";
            q.pop();
        }
        else{
            q.push(q.front());
            q.pop();
            now++;
        }
    }
}