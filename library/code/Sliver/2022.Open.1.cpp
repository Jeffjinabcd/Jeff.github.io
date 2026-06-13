#include <bits/stdc++.h>
#define LL long long
using namespace std;

struct interval {
    LL start, end;
};

int N, M;

interval p[100001];

bool cmp(const interval &a, const interval &b) { 
    return a.start < b.start;
}

bool check(LL mid) {
    LL previous = p[1].start;
    LL current = previous + mid;
    int cows = 1;
    int inter = 0;
    while (current <= p[M].end && inter <= M){
        while (current > p[inter].end){
            ++inter;
            if (inter > M){
                return cows >= N;
            }
        }
        ++cows;
        previous = max(p[inter].start, current);
        current = previous + mid;
    }
    return cows >= N;
}

int main() {
    // freopen("testing.in", "r", stdin);
    freopen("socdist.in", "r", stdin);
    freopen("socdist.out", "w", stdout);
    scanf("%d%d", &N, &M);
    for (int i = 1; i <= M; i++) {
        scanf("%lld%lld", &p[i].start, &p[i].end);
    }
    sort(p + 1, p + M + 1, cmp);
    LL l = 1, r = p[M].end, ans;
    while (l <= r) {
        LL mid = (l + r) / 2;
        if (check(mid)) {
            l = mid + 1;
            ans = mid;
        } else {
            r = mid - 1;
        }
    }
    printf("%lld\n", ans);
    return 0;
}