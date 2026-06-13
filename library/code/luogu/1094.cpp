#include <bits/stdc++.h>
using namespace std;
int a[30002],ans;
int main(){
    int w,n;
    cin>>w>>n;
    for(int i=0;i<n;i++)
        scanf("%d", &a[i]);
    sort(a,a+n);
    int i=0, j=n-1;
    while(i<=j){
        //如果两个能一起装下
        if(a[i]+a[j]<=w){
            i++;
        }
        j--;
        ans++;
    }
    cout<<ans<<endl;
    return 0;
}