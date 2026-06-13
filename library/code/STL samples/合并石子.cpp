#include <bits/stdc++.h>
using namespace std;

for (int i = 1; i <= n; ++i) {
    for (int j = 1; j <= n; ++j) {
        for (int k = i + 1; k < j - 1; ++k) {
            f[i][j]=min(f[i][j],f[i][k]+f[k+1][j]+s[j]-s[i-1]);
        }
    }
}
