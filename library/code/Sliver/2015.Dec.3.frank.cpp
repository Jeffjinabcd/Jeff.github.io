#include <bits/stdc++.h>
using namespace std;
int n,m;
int order[100010][3];
int x;
int a,b;
int main (){
    cin >> n;
    cin >> m;
    for(int i=1;i <= n;i++)
    {
        cin >> x;
        order[i][x]++;
    }
    for(int i=0;i <= m-1;i++)
    {
        cin >> a;
        cin >> b;
        cout << order[a][1] - order[b][1] << " " << order[a][2] - order[b][2] << " " << order[a][3] - order[b][3] << endl;
    }
}