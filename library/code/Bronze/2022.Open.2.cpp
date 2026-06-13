#include <bits/stdc++.h>
using namespace std;


int main() {
    //freopen("testing.in", "r", stdin);
    int N;
    cin >> N;
    vector<int> L;
    vector<int> G;
    char temp;
    int x;
    for (int i = 0; i < N; ++i){
        cin >> temp >> x;
        if (temp == 'L'){
            L.push_back(x);
        }
        else{
            G.push_back(x);
        }
    }
    G.push_back(1000000001);
    sort(L.begin(), L.end());
    sort(G.begin(), G.end());
    int j = 0, k = 0;
    int result = G.size() - 1;
    int test = G.size() - 1;
    for (int i = 0; i <= *L.rbegin(); ++i){
        if (i > L[j]){
            ++test;
            ++j;
        }
        if (i >= G[k]){
            --test;
            ++k;
        }
        if (test < result){
            result = test;
        }
        //cout << i << endl << test << endl << j << "  " << k << endl << endl;
    }
    cout << result;
}