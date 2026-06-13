#include <bits/stdc++.h>
using namespace std;

map<string, int> m;

int main() {
    // CRUD  增删改查
    int n;
    cin >> n;
    for (int i = 0; i < n; ++i) {
        string name;
        cin >> name;
        m[name]++;
    }
    string key;
    cin >> key;
    cout << m[key] << endl;
    m[key] += 5;  // 更新
    cout << m[key] << endl;
    m.erase(key);  // 删除
    // cout<<m[key]<<endl; // 执行这条语句之后m[key]的值会变为0
    cout<<m.count(key)<<endl;
    cout << m.size() << endl;

    // for each
    for (auto &kv : m) {
        cout << kv.first << " has value " << kv.second << endl;
    }

    // 利用iterator来循环
    map<string, int>::iterator itr = m.begin();
    while (itr != m.end()) {  // 元素未访问完
        // cout << (*itr).first << " " << (*itr).second << endl; //
        // 获取iterator当前位置里的值, 两种方式都可以用
        cout << itr->first << " " << itr->second << endl;
        itr++;  // 把iterator移动到下一个位置
    }

    cout << endl;

    return 0;
}
