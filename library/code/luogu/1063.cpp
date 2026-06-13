#include <bits/stdc++.h>
using namespace std;
#define ll long long

int n;
int a[220];
int f[440][440];

int main () {
    cin >> n;
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
        a[i + n] = a[i];
    }
    for(int i=2;i<=n+1;i++)
    {
        for(int l=1;l+i-1<=2*n;l++)
        {
            int r=l+i-1;
            for(int k=l+1;k<=l+i-2;k++)
                f[l][r]=max(f[l][r],f[l][k]+f[k][r]+a[l]*a[k]*a[r]); 
        }
    }
    int res=0;
    for (int i=1;i<=n;i++) res=max(res,f[i][n+i]);
    cout << res;
}