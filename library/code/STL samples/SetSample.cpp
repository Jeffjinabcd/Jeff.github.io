#include <bits/stdc++.h>
using namespace std;

// bool visited[100];
set<string> names;

int main() {
    int n;
    string s;
    cin>>n;
    for(int i=0; i<n; i++) {
        cin>>s;
        names.insert(s);  // 往set里添加一个数，如果有重复，则覆盖
    }

    cout<<names.size()<<endl; // 取set中的元素个数
    cout<<"================"<<endl;
    cin>>s;
    cout<<names.count(s)<<endl; // 统计set中有几个s,结果只可能为0或1
    names.erase("hello"); // 删除
    cout<<(names.find(s) != names.end())<<endl;
    cout<<"================"<<endl;

    // for each
    for (auto &k : names) {
        cout << k << endl;
    }
    cout << "================" << endl;
    set<string>::iterator itr = names.begin();
    while (itr != names.end())
    {
        cout<<*itr<<endl;    // 取出iterator指向的值
        itr++;
    }

    return 0;
}