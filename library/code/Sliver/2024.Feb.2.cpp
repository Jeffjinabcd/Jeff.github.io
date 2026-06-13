#include <algorithm>
#include <cstdio>
#include <vector>
int TC, N1, N2, N3, N, TY, a1[200001], a2[200001];
int getdig() {
    char c = getchar();
    while (c < '0' || c > '9') c = getchar();
    return c - '0';
}
int main() {
    //	freopen("s2.in", "r", stdin);
    for (scanf("%d", &TC); TC--;) {
        scanf("%d%d", &N, &TY);
        N1 = N2 = N3 = 0;
        for (int i = 1, x; i <= N; i++) {
            x = getdig();
            if (N1 == 0 || a1[N1] != x) a1[++N1] = x;
        }
        for (int i = 1, x; i <= N; i++) {
            x = getdig();
            if (N2 == 0 || a2[N2] != x) a2[++N2] = x;
        }
        std::vector<std::pair<int, int> > ans;
        int swap12 = 0;
        if (N1 != 1 && N2 == 1) {
            for (int i = 1; i <= N1; i++) {
                std::swap(a1[i], a2[i]);
            }
            std::swap(N1, N2);
            swap12 = 1;
        }
        if (N1 == 1) {
            if (N2 > 1) {
                while (N2 > 1 || N2 == 1 && a1[1] == a2[1]) {
                    if (a2[N2] == a1[1])
                        ans.push_back({2, 1});
                    else
                        ans.push_back({2, 3}), N3 = 1;
                    N2--;
                }
                if (N3) ans.push_back({3, 2});
            }
        } else {
            if (a1[1] != a2[1]) {
                int ty3 = a1[1];
                while (N1 > 1 || N2 > 1) {
                    if (a1[N1] != ty3 && a2[N2] != ty3) {
                        if (N2 > 1)
                            ans.push_back({2, 1}), N2--;
                        else
                            ans.push_back({1, 2}), N1--;
                    } else if (a2[N2] == ty3)
                        ans.push_back({2, 3}), N2--, N3 = 1;
                    else
                        ans.push_back({1, 3}), N1--, N3 = 1;
                }
                if (N3) ans.push_back({3, 1});
            } else {
                int ty3 = a1[1];
                while (N1 > 1 || N2 > 0) {
                    if (a1[N1] != ty3 && (!N2 || a2[N2] != ty3)) {
                        if (N2 > 0)
                            ans.push_back({2, 1}), N2--;
                        else
                            ans.push_back({1, 2}), N1--;
                    } else if (N2 && a2[N2] == ty3)
                        ans.push_back({2, 3}), N2--, N3 = 1;
                    else
                        ans.push_back({1, 3}), N1--, N3 = 1;
                }
                if (N3) ans.push_back({3, 1});
            }
        }
        if (swap12)
            for (auto &e : ans) {
                if (e.first == 1)
                    e.first = 2;
                else if (e.first == 2)
                    e.first = 1;
	
                if (e.second == 1)
                    e.second = 2;
                else if (e.second == 2)
                    e.second = 1;
            }
        printf("%d\n", int(ans.size()));
        if (TY != 1)
            for (auto &e : ans) printf("%d %d\n", e.first, e.second);
    }
}
