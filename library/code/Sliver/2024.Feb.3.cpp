#include <bits/stdc++.h>
using namespace std;

const int N = 3e5 + 5;
int a[N][5];
// 如K个数中奇偶数都有, 则猜奇数会损失偶数中的最大值, 反之同理
// b[i][0]，b[i][1]记录第i轮猜奇偶会失去多少
int b[N][4];

void solve() {
	int n, m, K;
	cin >> n >> m >> K;

	for (int i = 1; i <= m; ++i) {
		b[i][0] = b[i][1] = 0; // 分别存第i轮中奇数的最大值, 偶数的最大值
		b[i][2] = b[i][3] = 1001; // 分别存第i轮中奇数的最小值，偶数的最小值
	}

    // 如K个数中奇偶数都有, 则猜奇数会损失偶数中的最大值, 反之同理
	for (int i = 1; i <= m; ++i) {
		for (int j = 1; j <= K; ++j) {
			cin >> a[i][j];
			if(a[i][j] % 2 == 1){
				b[i][0] = max(b[i][0], a[i][j]);
				b[i][2] = min(b[i][2], a[i][j]);
			} else {
				b[i][1] = max(b[i][1], a[i][j]);
				b[i][3] = min(b[i][3], a[i][j]);
			}
		}
	}

	for (int i = 1; i <= m; ++i) {
		// 若无奇数, 猜偶数获得偶数的最小值(获得为负值)
		if (b[i][0] == 0) b[i][0] = -b[i][3];

		// 若无偶数, 猜奇数获得奇数的最小值
		if (b[i][1] == 0) b[i][1] = -b[i][2];
	}

	static int f[N];
	f[m + 1] = 1;
	for (int i = m; i > 0; --i)
		f[i] = max(1, f[i + 1] + min(b[i][0], b[i][1]));

	if (f[1] > n) {
		cout << "-1" << endl;
		return;
	}

	for (int i = 1; i <= m; ++i) {
		if (n >= b[i][0] + f[i + 1]) {
			n -= b[i][0];
			cout << "Even" << ((i == m) ? '\n' : ' '); // Even的字典序比Odd小
		}
		else {
			n -= b[i][1];
			cout << "Odd" << ((i == m) ? '\n' : ' ');
		}
	}
}

int main() {
	int T;
	cin >> T;
	while (T--) solve();

	return 0;
}

