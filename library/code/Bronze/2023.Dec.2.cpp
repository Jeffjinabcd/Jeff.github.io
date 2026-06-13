#include <bits/stdc++.h>
using namespace std;

int main () {
    //freopen("testing.in", "r", stdin);
    int N;
    cin >> N;
    string cows;
    cin >> cows;
    int beg = 0, en = 0, m = N, temp = 0, day;
    vector<int> groups(N);
    int i = 0;
    while (cows[i] == '1'){
        ++beg;
        ++i;
    }
    i = N - 1;
    while (cows[i] == '1'){
        ++en;
        --i;
    }
    i = 0;
    for (int j = beg; j < N - en; ++j){
        if (cows[j] == '1'){
            ++temp;
        }
        else{
            groups[i] = temp;
            ++i;
            if (m > temp && temp != 0){
                m = temp;
            }
            temp = 0;
        }
    }
    if (beg == N){
        cout << 1;
    }
    else{
        if (m % 2 == 1){
            --m;
        }
        else {
            m -= 2;
        }
        if (beg == 0 && en == 0){
            day = m/2;
        }
        else if (beg == 0 && en != 0){
            day = min(en - 1, m / 2);
        }
        else if (en == 0 && beg != 0){
            day = min(beg - 1, m / 2);
        }
        else{
            day = min(beg - 1, min(en - 1, m / 2));
        }
        int spread = day * 2 + 1;
        int result = 0;
        if (spread == 1){
            result += beg + en;
            for (int j = 0; j < N; ++j){
                result += groups[j];
            }
            cout << result;
        }
        else{
            if (beg < spread && beg != 0){
                ++result;
            }
            else{
                result += beg / spread;
                if (beg % spread != 0){
                    ++result;
                }
            }
            if (en < spread && en != 0){
                ++result;
            }
            else{
                result += en / spread;
                if (en % spread != 0 ){
                    ++result;
                }
            }
            for (int j = 0; j < N; ++j){
                result += groups[j] / spread;
                if (groups[j] % spread != 0){
                    ++result;
                }
            }
            cout << result;
        }
    }
}