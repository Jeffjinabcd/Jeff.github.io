#include <bits/stdc++.h>
using namespace std;

int n, m;

struct item {
    long long c[3], v[3];
}items[65];

long long value[32010];

int main () {
    cin >> n >> m;
    for (int i = 1; i <= m; ++i) {
        int c, v, temp;
        cin >> c >> v >> temp;
        if (temp == 0) {
            items[i].c[0] = c;
            items[i].v[0] = v * c;
        }
        else {
            if (items[i].c[1] == 0) {
                items[i].c[1] = c;
                items[i].v[1] = v * c;
            }
            else {
                items[i].c[2] = c;
                items[i].v[2] = v * c;
            }
        }
    }
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (j >= items[i].c[0]) {
                value[j] = max(value[j], value[j - items[i].c[0]] + items[i].v[0]);
            }
            if (j >= items[i].c[0] + items[i].c[1]) {
                value[j] = max(value[j], value[j - items[i].c[0] - items[i].c[1]] + items[i].v[0] + items[i].v[1]);
            }
            if (j >= items[i].c[0] + items[i].c[2]) {
                value[j] = max(value[j], value[j - items[i].c[0] - items[i].c[2]] + items[i].v[0] + items[i].v[2]);
            }
            if (j >= items[i].c[0] + items[i].c[1] + items[i].c[2]) {
                value[j] = max(value[j], value[j - items[i].c[0] - items[i].c[1] - items[i].c[2]] + items[i].v[0] + items[i].v[1] + items[i].v[2]);
            }
            value[j] = max(value[j], value[j - 1]);
        }
    }
    cout << value[n];
}