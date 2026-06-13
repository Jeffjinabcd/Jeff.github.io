#include <bits/stdc++.h>
using namespace std;

int stairs[32];

int main () {
    stairs[1] = 1;
    stairs[2] = 2;
    stairs[3] = 4;
    for (int i = 4; i < 32; ++i) {
        stairs[i] = stairs[i - 1] + stairs[i - 2] + stairs[i - 3];
    }
    int temp;
    while (cin >> temp) {
        if (temp != 0) {
            cout << stairs[temp] << endl;
        }
        else {
            return 0;
        }
    }
}