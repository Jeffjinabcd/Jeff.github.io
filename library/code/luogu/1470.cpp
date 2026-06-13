#include<bits/stdc++.h>
using namespace std;
map<string,bool>s;//一个map解千愁~就是<u>**速度慢了点**</u>，大佬请自行打**hash**
int x;
int ans=0;
int ams=0;
string l;
bool F=0;
bool come[200001];
inline void to_search(int now){	
    if(come[now]){//如果曾经搜索过此时的‘y’
        return;//退出
    }
    come[now]=1;//标记搜索过此时的‘y’
    if(now==x){
        return;
    }
    string qu="";
    for(int i=now;i<x;++i){
        if(i-now>=ams){//如果现在加的元素大于了最长元素，之后一定不可能成立，于是跳过（虽然是小优化，也很强大有不有~）
            return;
        }
        qu+=l[i];
        if(s[qu]){//如果P集合中有此时的元素
            F=1;//判断成立
            if(i>ans){//记录答案
                ans=i;
            }
            to_search(i+1);//从下一个y开始搜索
        }
    }
}
int main(){
    int e=1;
    while(cin>>l){
        if(l=="."){
            break;
        }
        s[l]=1;//标记元素存在
        if(l.size()>ams){
            ams=l.size();//记录最长元素（小优化第一步）
        } 
    }
    string yu="";
    while(cin>>l){//输入字符串S
        yu+=l;//将字符串S整合
    }
    l=yu;
    x=l.size();//得到S的长度
    to_search(0);//从第一个字符开始搜索
    if(!F){//注意判断是否搜索过，第一个点的坑QWQ
        printf("0");
        return 0;
    }
    printf("%d",ans+1);//输出答案（记得加一，因为是从0开始搜的）
    return 0;
}