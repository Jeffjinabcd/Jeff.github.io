#include <bits/stdc++.h>
using namespace std;

struct cow{
    int C, O, W;
};

cow record[200010];

int main (){
    // freopen("testing.in", "r", stdin);
    char temp1;
    int N;
    string str;
    cin >> str;
    int Q;
    cin >> Q;
    for (int i = 1; i <= str.length(); ++i){
        record[i] = record[i - 1];
        if (str[i - 1] == 'C'){
            ++record[i].C;
        }
        if (str[i - 1] == 'O'){
            ++record[i].O;
        }
        if (str[i - 1] == 'W'){
            ++record[i].W;
        }
    }
    int s, e;
    int C, O, W;
    for (int i = 0; i < Q; ++i){
        cin >> s >> e;
        C = (record[e].C - record[s - 1].C) % 2;
        O = (record[e].O - record[s - 1].O) % 2;
        W = (record[e].W - record[s - 1].W) % 2;
        if ((C == 1 && (O == 0 && W == 0)) || (C == 0 && (O == 1 && W == 1))){
            cout << "Y";
        }
        else{
            cout << "N";
        }

    }
}