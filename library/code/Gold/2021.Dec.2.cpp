#include <bits/stdc++.h>
using namespace std;
const int MAXN = 2e5 + 5;
int n;
int a[MAXN];
int p[MAXN];
int l[MAXN];
int r[MAXN];
set<int> s;
void dfs(int x, int d) {
    if (x == 0) {
        cout << d/2 << endl;
        return;
    }
    if (d % 2 == 0) {
        dfs(l[x], d + 1);
        dfs(r[x], d);
    }
    else {
        dfs(l[x], d);
        dfs(r[x], d + 1);
    }
}

/*
样例 5 1 2 4 3 的二叉树(参见U2021decg2.png)
5 左孩子 1     右孩子 5.5
1 左孩子 0.5   右孩子 2
2 左孩子 1.5   右孩子 4
4 左孩子 3     右孩子 4.5
3 左孩子 2.5   右孩子 3.5

构造一个二叉树，直接DFS就可以了

具体来说是，每个元素找在自己之前大于自己最小的元素和小于自己最大的元素，
看哪个离自己更近（更靠后）
根据自己和这个元素的关系，决定是挂在这个元素的左边还是右边

最后DFS，d表示从根节点到当前节点 HI LO HI LO ... 序列
（如果d是奇数，那么这个序列以 HI 结尾 
如果到了叶子节点 d / 2 就是答案
如果d是偶数且往左（HI）走， d += 1
如果d是奇数且往左（HI）走，连续出现多个HI，d不变
如果d是奇数且往右（LO）走，d += 1
如果d是偶数且往右（LO）走，连续出现多个LO，d不变
*/
int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a[i]);
        p[a[i]] = i;
    }
    s.insert(a[1]);
    for (int i = 2; i <= n; i++) {
        auto it = s.lower_bound(a[i]);
        int u = -1;
        if (it != s.end()) {
            u = *it;
        }
        if (it != s.begin()) {
            it--;
            if (u == -1 || p[u] < p[*it]) {
                u = *it;
            }
        }
        if (a[i] < u) {
            l[u] = a[i];
        }
        else if (a[i] > u) {
            r[u] = a[i];
        }
        s.insert(a[i]);
    }
    dfs(a[1], 0);
    return 0;
}