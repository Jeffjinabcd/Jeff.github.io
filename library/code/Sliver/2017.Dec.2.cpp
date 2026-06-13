#include <bits/stdc++.h>
using namespace std;

struct record {
    int day;
    int cow;
    int delta;
};

record A[100001];

bool cmp(const record &a, const record &b) { 
    return a.day < b.day;
}

int main() {
    // freopen("measurement.in", "r", stdin);
    // freopen("measurement.out", "w", stdout);
    ios::sync_with_stdio(false);
    cin.tie(0);

    int N, G;
    cin >> N >> G;

    for (int i = 0; i < N; i++) {
        cin >> A[i].day >> A[i].cow >> A[i].delta;
    }
    sort(A, A + N, cmp);

    // key：牛的编号  value: 对应的牛产奶变化量
    map<int, int> everyCow;
    // key：总的产奶变化量  value 对应的牛的头数
    map<int, int, greater<int> > totalChange;  // 按总的产奶变化量降序排列
    // 请注意，农夫约翰有一大群奶牛。
    // 所以尽管日志中记录了一些奶牛改变了产奶量，
    // 但仍然还有很多奶牛的产奶量保持在G加仑。
    totalChange[0] = N + 1;

    int ans = 0;
    for (int i = 0; i < N; i++) {
        // 检查在变化之前，当前这头牛是否是冠军，以及和这头牛产奶量一样的牛的数量
        int milk = everyCow[A[i].cow];

        bool wasTop = (milk == totalChange.begin()->first);  //变化前是否为冠军
        int wasCnt = totalChange[milk];

        // 更新milk对应的奶牛头数
        if (wasCnt == 1) {
            totalChange.erase(milk);
        } else if (wasCnt > 1) {
            totalChange[milk]--;
        }

        // 更新产奶量
        milk += A[i].delta;
        everyCow[A[i].cow] = milk;
        totalChange[milk]++;

        bool isTop = (milk == totalChange.begin()->first);  //变化后是否为冠军
        int isCnt = totalChange[milk];

        if (wasTop != isTop) {
            ans++;
        } else if (wasTop && isTop && wasCnt != isCnt) {
            // 变化前后产量最高的牛数量不一样
            ans++;
        }
    }
    cout << ans << endl;

    return 0;
}
