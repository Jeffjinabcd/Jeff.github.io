#include <bits/stdc++.h>
using namespace std;
// f[i][j][k]表示当前枚举到第i行，ｊ表示当前行的状态，ｋ表示上一行的状态
int n, m, H[105], f[105][66][66], sta[70], cnt, counting[200];

int main() {
    // n行m列
    cin >> n >> m;
    char a;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            cin >> a;
			// 把每行“不可选”状态压缩起来
			if (a == 'H') {
				H[i] = (H[i] << 1) + 1;
			} else {
				H[i] = (H[i] << 1);
			}
        }
    }

    sta[++cnt] = 0;
    for (int i = 1; i < (1 << m); i++) {
        // 左二右二不能选
        if (i & (i << 1)) continue;
        if (i & (i << 2)) continue;
        sta[++cnt] = i;  // 直接存有用的就行
        int x = i;
        while (x) {
            counting[cnt]++; // 统计1的个数
            x -= (x & (-x)); // 去掉最后一个1
        }
    }

    for (int i = 1; i <= cnt; i++) { //第一行
        if ((sta[i] & H[1]) == 0) {  //不能与地形冲突
            f[1][i][1] = counting[i];
        }
    }
    
	if(n > 1){
		for (int i = 1; i <= cnt; i++) {  //第二行
			if ((sta[i] & H[2]) == 0) {
				for (int j = 1; j <= cnt; j++) {
					//判断是否冲突
					if ((sta[i] & sta[j]) == 0 && (sta[j] & H[1]) == 0) {
						f[2][i][j] = counting[j] + counting[i];
					}
				}
			}
		}
	}

    // 枚举状态
    for (int i = 3; i <= n; i++) {
        for (int j = 1; j <= cnt; j++) { // 第i行状态
            if ((sta[j] & H[i]) == 0) {
                for (int k = 1; k <= cnt; k++) {  // 第i-1行状态
                    if ((sta[j] & sta[k]) == 0 && (sta[k] & H[i - 1]) == 0) {
                        for (int k2 = 1; k2 <= cnt; k2++) {  // 第i-2行状态
                            // 判断所有冲突情况
                            if ((sta[j] & sta[k2]) == 0 &&
                                (sta[k] & sta[k2]) == 0 &&
                                (sta[k2] & H[i - 2]) == 0) {
                                f[i][j][k] = max(f[i][j][k], f[i - 1][k][k2] + counting[j]);
                            }
                        }
                    }
                }
            }
        }
    }

    int ans = 0;
    for (int i = 1; i <= cnt; i++) {
        for (int j = 1; j <= cnt; j++) {
            ans = max(ans, f[n][i][j]);
        }
    }
    cout << ans;
    return 0;
}
