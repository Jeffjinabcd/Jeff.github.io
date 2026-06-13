#include<iostream>
#include<cstring>
#include<iostream>
#define maxn 21
using namespace std;
int N,T,M;
int f[maxn][maxn];//用于保存状态
int t[maxn];
int max(int i,int j,int k){
    i=max(i,j);
    i=max(i,k);
    return i;
}
int main(){
    cin>>N>>T>>M;
    for(int i=1;i<=N;i++){
        cin>>t[i];
    }
    for(int i=0;i<=T;i++){//初始化数组
        f[0][i]=0;
    }
    for(int i=1;i<=N;i++){
        for(int m=M;m>=1;m--){//注意：因为是01背包，所以要从后往前更新状态
            for(int j=T;j>=t[i];j--){
                f[m][j]=max(f[m][j],f[m-1][T]+1,f[m][j-t[i]]+1);//状态转移
            }
        }
    }
    cout<<f[M][T];
}