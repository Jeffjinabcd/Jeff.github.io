#include <bits/stdc++.h>
using namespace std;

const int MAXN = 5000;
const int MOD  = 1e9 + 7;
int f[MAXN + 1][MAXN + 1];  // f[i][j]��ʾ����Ϊi�ľ��ӣ������һ�����������Ϊj�ķ�������
int s[MAXN + 1]; // s[i]��ʾ����Ϊi�ľ��ӵķ�����
int len[MAXN + 1]; // ��i�����ʵĳ���Ϊlen[i]
int r[MAXN + 1]; // ��i�����ʵ��ϲ�Ϊr[i]
int cnt[26];

inline int fastpow(int x, int k) {
	int ans = 1;
	while(k) {
		if(k & 1) {
            ans = (1LL * ans * x) % MOD;
        }
		x = (1LL * x * x) % MOD;
		k >>= 1;
	}
	return ans;
}

/*
��������
����ʫ֮���໥Ӱ���ֻ�����һ����������ɣ�O(NK)
*/
int main() {
    freopen("poetry.in", "r", stdin);
    freopen("poetry.out", "w", stdout);
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, m, k; // n������ ÿ��ʫM�� ÿ��K������
    cin>>n>>m>>k;
    for (int i = 1; i <= n; i++) {
        cin >> len[i] >> r[i];  // ��i�����ʵĳ��ȡ��ϲ�
    }
    char c;
    for (int i = 1; i <= m; i++) {
        cin >> c;
        cnt[c - 'A']++;
    }

    s[0] = 1;  // s[i]��ʾ����Ϊi�ľ��ӵķ�����
    for (int i = 1; i <= k; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (i >= len[j]) {
                f[i][r[j]] = (f[i][r[j]] + s[i - len[j]]) % MOD;
                s[i] = (s[i] + s[i - len[j]]) % MOD;
            }
        }
    }

    long long  ans = 1;
    for (int i = 0; i < 26; ++i) {
        if (!cnt[i]) {
            continue;
        }
        int sum = 0;
        for (int j = 1; j <= n; ++j) {
            if (f[k][j]) {
                sum = (sum + fastpow(f[k][j], cnt[i])) % MOD;
            }
        }
        ans = (ans * sum) % MOD;
    }
    cout << ans << endl;
    return 0;
}
