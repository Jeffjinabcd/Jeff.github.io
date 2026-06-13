#include <bits/stdc++.h>
using namespace std;

int chocolate[25];

int main () {
    chocolate[1] = 1;
    chocolate[2] = 2;
    for (int i = 3; i < 22; ++i) {
        chocolate[i] = chocolate[i - 1] + chocolate[i - 2];
    }
    int temp;
    cin >> temp;
    cout << chocolate[temp];
}