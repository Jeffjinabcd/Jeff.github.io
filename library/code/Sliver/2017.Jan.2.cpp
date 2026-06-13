#include <bits/stdc++.h>
using namespace std;

struct gestures{
    int H, P, S;
};

gestures record[100010];

int main (){
    // freopen("testing.in", "r", stdin);
    freopen("hps.in", "r", stdin);
    freopen("hps.out", "w", stdout);
    int N;
    cin >> N;
    char temp;
    for (int i = 1; i <= N; ++i){
        cin >> temp;
        record[i] = record[i - 1];
        if (temp == 'H'){
            ++record[i].H;
        }
        if (temp == 'P'){
            ++record[i].P;
        }
        if (temp == 'S'){
            ++record[i].S;
        }
    }
    int result = 0, temp1;
    for (int i = 1; i <= N; ++i){
        temp1 = max(record[i].H, max(record[i].P, record[i].S)) + max(record[N].H - record[i].H, max(record[N].P - record[i].P, record[N].S - record[i].S));
        // cout << temp1 << endl;
        result = max(temp1, result);
    }
    cout << result;
}