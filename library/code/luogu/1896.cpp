#include <bits/stdc++.h>
using namespace std;
long long counting[2005], sit[2005];
// f[i][j][l]��ʾֻ����ǰi�У���i�а��ձ��Ϊj��״̬���ù��������Ѿ�����l������ʱ�ķ�������
long long f[15][2005][105];
int n, k, cnt;

void dfs(int x, int kingNum, int curPos) { // x��ʾ��ǰ��״̬�� num��ʾ�������� curPos��ʾλ��
    if (curPos >= n) {
        sit[++cnt] = x;
        counting[cnt] = kingNum;
        return;
    }
    dfs(x, kingNum, curPos + 1);
    dfs(x + (1 << curPos), kingNum + 1, curPos + 2);
}

int main() {
    cin >> n >> k;
    dfs(0, 0, 0);  // ��Ԥ����һ�е����кϷ�״̬
    for (int i = 1; i <= cnt; i++) {
        f[1][i][counting[i]] = 1;
    }

    for (int i = 2; i <= n; i++) // ��i��
        for (int j = 1; j <= cnt; j++) { // ��i�й���״̬���
            for (int l = 1; l <= cnt; l++) {  // ��i-1�й���״̬���
                if (sit[j] & sit[l]) {
                    continue;
                }
                if ((sit[j] << 1) & sit[l]) {
                    continue;
                }
                if (sit[j] & (sit[l] << 1)) {
                    continue;
                }

                for (int p = counting[j]; p <= k; ++p) {
                    f[i][j][p] += f[i - 1][l][p - counting[j]];
                }
            }
        }

    long long ans = 0;
    for(int i=1;i<=cnt;i++)ans+=f[n][i][k];
    cout << ans << endl;
    return 0;
}