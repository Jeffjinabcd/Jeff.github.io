#include <bits/stdc++.h>
using namespace std;

vector<int> vec;

/*
input
5
1 2 3 4 5

output
vector初始分配空间大小0
max size: 4611686018427387903
5
1 2 3 4 5
调用reserve之前8
调用reserve之后20
11 2 3 4 5 0 1 2 3 4 5 6 7
11 2 3 4 5 0 1 2 3 4 5 6 7 -1 -1
调用resize之后20
2 10 3 4 5 0 1 2 3 4 5 6 7 -1 -1
2 10 3 4 5 0 1 2 3 4 5 6 7 -1 -1
vec剩余元素个数：0
 */
int main() {
    cout << "vector初始分配空间大小" << vec.capacity() << endl;
    // vector
    cout << "max size: " << vec.max_size() << endl;
    int n, t;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> t;
        vec.push_back(t);  // 增加一个数到末尾
    }
    cout << "调用reserve之前" << vec.capacity() << endl;
    vec.reserve(10);  // 预分配空间大小， 提升vector性能, 之后如超过10，则倍增变20
    for (int i = 0; i < 8; i++) {
        vec.push_back(i);
    }
    cout << "调用reserve之后" << vec.capacity() << endl;

    vec[0] += 10;  // 更新

    // 利用下标来循环
    for (int i = 0; i < vec.size(); i++) {
        cout << vec[i] << " ";  // 读取
    }
    cout << endl;
    vec.resize(15, -1);                     // 调整元素个数
    for (int i = 0; i < vec.size(); i++) {  // 获取当前vector中元素个数
        cout << vec[i] << " ";              // 读取
    }
    cout << "\n调用resize之后" << vec.capacity() << endl;

    vec.insert(vec.begin() + 2, 10);  // 把10插入到vec[2]上
    vec.erase(vec.begin());           // 删除vec[0]
    for (int i = 0; i < vec.size(); i++) {
        cout << vec[i] << " ";
    }
    cout << endl;

    // for each
    for (auto &e : vec) {  // for (int &e : vec) {  也可以把int替换为auto
        cout << e << " ";
    }
    cout << endl;

    // 利用iterator来循环
    vector<int>::iterator itr = vec.begin();
    while (itr != vec.end()) {  // 元素未访问完
        cout << *itr << " ";    // 获取iterator当前位置里的值
        itr++;                  // 把iterator移动到下一个位置
    }
    cout << endl;

    // 清空, 删除vector中的全部元素
    vec.clear();
    cout << "vec剩余元素个数：" << vec.size() << endl;
    return 0;
}
