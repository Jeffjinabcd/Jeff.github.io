#include <bits/stdc++.h>
using namespace std;

int N, Q, prefix[100002], suffix[100002];
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    cin >> N >> Q;
    string s;
    cin >> s;
    for (int c = 'A'; c <= 'Z'; c++) {
        int cnt = 0;
        bool brushdown = false; // false表示当前刷子提起状态
        for (int i = 1; i <= N; i++) {
            if (s[i-1] == c) {
                if (!brushdown) {
                    cnt++;
                    brushdown = true;
                }
            }
            else if (s[i - 1] < c) {
                brushdown = false;
            }
            prefix[i] += cnt; // 26种颜色的前缀和累加
        }

        cnt = 0;
        brushdown = false;
        for (int i = N; i > 0; i--) {
            if (s[i - 1] == c) {
                if (!brushdown) {
                    cnt++;
                    brushdown = true;
                }
            }
            else if (s[i - 1] < c) {
                brushdown = false;
            }
            suffix[i] += cnt;
        }
    }

    for (int i = 1; i <= Q; i++) {
        int a, b;
        cin >> a >> b;
        cout << prefix[a - 1] + suffix[b + 1] << endl;
    }
    return 0;
}
