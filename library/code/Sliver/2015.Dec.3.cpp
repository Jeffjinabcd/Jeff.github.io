#include <bits/stdc++.h>
using namespace std;

struct cows {
    int H, G, J;
};

cows prefix[100001];

int main(){
    // freopen("testing.in", "r", stdin);
	freopen("bcount.in", "r", stdin);
	freopen("bcount.out", "w", stdout);

    int N, Q, temp;
    cin >> N >> Q;
    for (int i = 1; i <= N; ++i){
        cin >> temp;
        prefix[i] = prefix[i - 1];
        if (temp == 1){
            prefix[i].H++;
        }
        if (temp == 2){
            prefix[i].G++;
        }
        if (temp == 3){
            prefix[i].J++;
        }
    }
    int s, e;
    for (int i = 0; i < Q; ++i){
        cin >> s >> e;
        cout << prefix[e].H - prefix[s - 1].H << " " << prefix[e].G - prefix[s - 1].G << " " << prefix[e].J - prefix[s - 1].J << endl;
    }
}