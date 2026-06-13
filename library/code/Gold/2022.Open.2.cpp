#include <bits/stdc++.h>
using namespace std;
const int p = 1e9 + 7;
int T, n;
string a, b;
int f[2005][2005];  // f[i][j]表示最后一个加入的是 a[i-1] 的方案数
int g[2005][2005];  // g[i][j]表示最后一个加入的是 b[j-1] 的方案数

string get() {
    string s, t;
    cin >> s;
    for (char c : s) {
        if (c == '0') { // 0前面的都可以忽略
            t = "0";
        } else if (c != '1') { // 所有的1都可以忽略
            t += c;
        }
    }
    return t;
}

/*
这是一个动态规划
首先对于两个字符串，最后一个0前面的都可以忽略，所有的1都可以忽略
然后动态规划，问归并两个字符串有多少个方案
f[i][j] 表示最后一个加入的是 a[i-1] 的方案数
g[i][j] 表示最后一个加入的是 b[j-1] 的方案数

四种转移，其中三种是在任意情况（当然i和j要合法）下都可以做的
f[i + 1][j] = (f[i + 1][j] + f[i][j]) % p;
g[i][j + 1] = (g[i][j + 1] + g[i][j]) % p;
g[i][j + 1] = (g[i][j + 1] + f[i][j]) % p;

在 b[j-1] 后面接 a[i] 要求 （b[j-1] 和 a[i] 不能都是数字）或 （b[j-1] 和 a[i] 不能都是加号）
f[i + 1][j] = (f[i + 1][j] + g[i][j]) % p;

换句话说，在最终生成的程序中
不能出现 b的加号 后面接着 a的加号
不能出现 b的数字 后面接着 a的数字
允许出现 a的任何字符 后面接着 a的任何字符
允许出现 b的任何字符 后面接着 b的任何字符
允许出现 a的数字 后面接着 b的数字
允许出现 b的数字 后面接着 a的加号
允许出现 a的加号 后面接着 b的加号
允许出现 b的加号 后面接着 a的数字
*/
int main() {
    ios::sync_with_stdio(false);
	cin.tie(0), cout.tie(0);
    cin >> T;
    for (int t = 0; t < T; t++) {
        cin >> n;
        a = get();
        b = get();
        memset(f, 0, sizeof f);
        memset(g, 0, sizeof g);
        f[1][0] = 1;
        g[0][1] = 1;
        for (int i = 0; i <= a.size(); i++) {
            for (int j = 0; j <= b.size(); j++) {
                if (i == 0 && j == 0) {
                    continue;
                }
                if (i < a.size()) {
                    f[i + 1][j] = (f[i + 1][j] + f[i][j]) % p;
                    if (j > 0 && b[j - 1] != '+' && a[i] == '+') {
                        f[i + 1][j] = (f[i + 1][j] + g[i][j]) % p;
                    }
                    if (j > 0 && b[j - 1] == '+' && a[i] != '+') {
                        f[i + 1][j] = (f[i + 1][j] + g[i][j]) % p;
                    }
                }
                if (j < b.size()) {
                    g[i][j + 1] = (g[i][j + 1] + g[i][j]) % p;
                    g[i][j + 1] = (g[i][j + 1] + f[i][j]) % p;
                }
            }
        }
        cout << (f[a.size()][b.size()] + g[a.size()][b.size()]) % p << endl;
    }
    return 0;
}