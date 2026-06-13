#include <bits/stdc++.h>
using namespace std;

int main() {
    //freopen("testing.in", "r", stdin);
    int N;
    cin >> N;
    string GH;
    cin >> GH;
    int times = 0;
    bool current = true;
    for (int i = 0; i < N; i += 2){
        if (GH.substr(i, 2) == "GH"){
            if (times == 0){
                ++times;
                current = false;
            }
            else if (current){
                ++times;
                current = false;
            }
        }
        if (GH.substr(i, 2) == "HG"){
            if (times == 0){
                ++times;
                current = true;
            }
            else if (!current){
                ++times;
                current = true;
            }
        }
    }
    cout << times - current;
}