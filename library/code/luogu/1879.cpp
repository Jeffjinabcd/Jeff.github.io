#include <bits/stdc++.h>
using namespace std;
const int MOD = 1e8;
int m, n, f[13][4096], sta[13], field[13][13];
bool valid[4096];
int main() {
    // m��n��
    cin >> m >> n;
    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++) cin >> field[i][j];

    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++) {
		    sta[i] = (sta[i] << 1) + field[i][j];
		    
	    }

    int cnt = 1 << n;
    for (int i = 0; i < cnt; i++) {
        if ((i & (i << 1)) == 0) {
            valid[i] = true;
        }
    }

    f[0][0] = 1;
    for (int i = 1; i <= m; i++)
        for (int j = 0; j < cnt; j++) {
            if (valid[j] && (j & sta[i]) == j) {
                for (int k = 0; k < cnt; ++k) {
                    if ((k & j) == 0) {
                        f[i][j] = (f[i][j] + f[i - 1][k]) % MOD;
                    }
                }
            }
        }

    int ans = 0;
    for (int i = 0; i < cnt; i++) {
        ans += f[m][i];
        ans %= MOD;
    }
    cout << ans << endl;
    return 0;
}
