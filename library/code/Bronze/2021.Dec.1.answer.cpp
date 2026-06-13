#include <bits/stdc++.h>
using namespace std;
const int N = 5e5 + 1;
string s;

// preg[i] : 记录左侧最近一个G的位置
// preh[i] : 记录左侧最近一个H的位置
// nexg[i] : 记录右侧最近一个G的位置
// nexh[i] : 记录右侧最近一个H的位置
int preg[N], preh[N], nexg[N], nexh[N];
int main() {

    int n;
    cin >> n;
    cin >> s;

    for (int i = 1; i <= n; i++) {
        if (s[i - 1] == 'G') {
            preg[i + 1] = i;
            preh[i + 1] = preh[i];
        } else {
            preh[i + 1] = i;
            preg[i + 1] = preg[i];
        }
    }

    nexg[n] = nexh[n] = n + 1;
    for (int i = n; i >= 1; i--) {
        if (s[i - 1] == 'G') {
            nexg[i - 1] = i;
            nexh[i - 1] = nexh[i];
        } else {
            nexh[i - 1] = i;
            nexg[i - 1] = nexg[i];
        }
    }

    long long ans = 0;
    for (int i = 1; i <= n; i++) {
        if (s[i - 1] == 'G') {
            ans += 1ll * (nexg[i] - i) * (i - preg[i]);
        } else {
            ans += 1ll * (nexh[i] - i) * (i - preh[i]);
        }
    }

    for (int i = 0; i < (n - 1); i++) {
        // 把长度为2的去掉
        if (s[i] != s[i + 1]) {
            ans -= 2;  // -2是由于GH和HG都会被统计两次
        }
    }

    cout << ans - n;  // 把长度为1的去掉
    return 0;
}
