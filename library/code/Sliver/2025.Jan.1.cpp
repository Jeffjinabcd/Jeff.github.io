#include <bits/stdc++.h>
using namespace std;

int N, a[500001], b[500001];
vector<int> A[500001], B[500001];
int main() {
    scanf("%d", &N);
    for (int i = 1; i <= N; i++) {
        scanf("%d", a + i);
        A[a[i]].push_back(min(i, N - i + 1));
    }
    for (int i = 1; i <= N; i++) {
        scanf("%d", b + i);
        B[b[i]].push_back(min(i, N - i + 1));
    }
    long long ans = 0;
    for (int i = 1; i <= N; i++) {
        if (a[i] == b[i]) {
            ans += (long long)(i - 1) * i / 2 + (long long)(N - i + 1) * (N - i) / 2;
        }
    }        

    for (int e = 1; e <= N; e++) { // every pair
        sort(A[e].begin(), A[e].end(), greater<int>());
        sort(B[e].begin(), B[e].end(), greater<int>());
        for (int i = 1; i <= (N + 1) / 2; ++i) {
            while (!A[e].empty() && A[e].back() < i) A[e].pop_back();
            while (!B[e].empty() && B[e].back() < i) B[e].pop_back();
            if (A[e].empty() || B[e].empty()) break;
            int jump = min(A[e].back(), B[e].back()) - i;
            ans += (long long)A[e].size() * B[e].size() * (jump + 1);
            i += jump;
        }
    }

    printf("%lld\n", ans);
}
