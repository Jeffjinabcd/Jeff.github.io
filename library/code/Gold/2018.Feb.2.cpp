#include <bits/stdc++.h>
#define LL long long
using namespace std;

const int L = 1e5 + 5;
LL n, leaf, ans;  // leaf:�ܵ�Ҷ�ڵ�ĸ���  ans:�����ļ������·���ĳ���֮�͵���Сֵ
LL len[L], dis[L];  // len:�ļ��л��ļ����ֳ���  dis:�����ڵ��·������
LL leafcnt[L], sum[L];  // leafcnt:Ҷ�ڵ����Ŀ  sum:���·������֮��
vector<int> son[L];     // son:���ӽڵ�

// ����Ӹ��ڵ㵽������������Ϣ
void dfs1(int x) {
    if (son[x].size() == 0)  // xΪfile
    {
        leafcnt[x] = 1;
        dis[x]--;  // ȥ������б�ܵĳ���
        sum[1] += dis[x];
        return;
    }

    for (int id : son[x]) {
        dis[id] = dis[x] + len[id] + 1;
        dfs1(id);
        leafcnt[x] += leafcnt[id];
    }
}

// ���
void dfs2(int x) {
    for (int id : son[x]) {
        if (son[id].size() == 0) continue; // idΪ�ļ�
        // ���ڵ�����ӽڵ�ĵ��ƹ�ϵʽ
        // sum[x]�Ǹ��ڵ�Ĵ� sum[id]���ӽڵ�Ĵ�
        // (len[id]+1)*leafcnt[id] ��ʾ��x�㵽id��ľ��루����ȻҪ��ȥ��
        // 3*(leaf-leafcnt[id]) ��ʾ��id��xҪ�Ӷ���
        sum[id] = sum[x] - (len[id] + 1) * leafcnt[id] + 3 * (leaf - leafcnt[id]);
        ans = min(ans, sum[id]);
        dfs2(id);
    }
}

int main() {
    freopen("dirtraverse.in", "r", stdin);
    freopen("dirtraverse.out", "w", stdout);
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);

    cin >> n;
    for (int i = 1; i <= n; i++) {
        LL m;
        string s;
        cin>>s;
        len[i] = s.length();
        cin >> m;
        if (!m) leaf++;
        for (int j = 1; j <= m; j++) {
            LL t;
            cin >> t;
            son[i].push_back(t);
        }
    }
    dfs1(1);
    ans = sum[1];
    dfs2(1);
    cout<<ans<<endl;

    return 0;
}
