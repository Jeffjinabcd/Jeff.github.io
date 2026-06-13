#include <bits/stdc++.h>
using namespace std;

const int N = 1e5 + 1;
int n, a[N];
int first[N], last[N];  // first[i]��last[i]�ֱ����i����������ֵ�λ��
int s[N], t[N];  // s��ʾ��ǰλ������һ����Ŀ�ͷ��t��ʾ��ǰλ������һ����Ľ�β
int ans = 0;  // top��ʾջ��
stack<int> st;

int main() {
    freopen("art2.in","r",stdin);
    freopen("art2.out","w",stdout);
    ios::sync_with_stdio(false);
    cin.tie(0);

    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
        if (a[i] == 0) continue;

        if (first[a[i]] == 0) {
            first[a[i]] = i;
        }
        last[a[i]] = i;
    }

    for (int i = 1; i <= n; i++) {
        if (first[i] != 0) s[first[i]] = i;
        if (last[i] != 0) t[last[i]] = i;
    }

    for (int i = 1; i <= n; i++) {
        if (s[i]) {
            st.push(a[i]);
        }
        ans = max(ans, (int)st.size());
        
        if (!st.empty() && st.top() != a[i]) {
            cout << -1 << endl;
            return 0;
        }
        
        if (t[i]) {
            st.pop();
        }
    }
    cout << ans << endl;  //���
    return 0;
}