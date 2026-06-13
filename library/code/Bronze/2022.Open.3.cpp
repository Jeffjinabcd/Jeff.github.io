#include <bits/stdc++.h>
using namespace std;

struct recipe{
    int L;
    int M;
    vector<int> Ms;
};

bool sortingg(const recipe& x, const recipe& y){
    return x.L > y.L;
}

bool possible (int x, vector<recipe>& o, vector<int>& p){
    //cout << x << endl;
    if (p[x - 1] > 0){
        --p[x - 1];
        //cout << "exist" << endl << endl;
        return true;
    }
    int i = 0;
    while (i < o.size()){
        if (o[i].L == x){
            //cout << "recipe" << endl;
            for (int j = 0; j < o[i].M; ++j){
                if (!possible(o[i].Ms[j], o, p)){
                    //cout << "failed" << endl;
                    return false;
                }
            }
            return true;
        }
        ++i;
    }
    return false;
}

int main() {
    //freopen("testing.in", "r", stdin);
    int N;
    cin >> N;
    vector<int> metals(N);
    vector<recipe> recipes;
    for (int i = 0; i < N; ++i){
        cin >> metals[i];
    }
    int K;
    cin >> K;
    int L, M;
    for (int i = 0; i < K; ++i){
        cin >> L >> M;
        vector<int> l(M);
        for (int j = 0; j < M; ++j){
            cin >> l[j];
        }
        recipes.push_back({L, M, l});
    }
    sort(recipes.begin(), recipes.end(), sortingg);
    /*
    for (int i = 0; i < K; ++i){
        cout << recipes[i].L << "\t" << recipes[i].M << endl;
        for (int j = 0; j < recipes[i].M; ++j){
            cout << recipes[i].Ms[j] << "\t";
        }
        cout << endl << endl << endl;
    }
    */
    int result = 0;
    bool quit = true;
    if (recipes[0].L != N){
        cout << metals[N - 1];
    }
    else{
        while (possible(N, recipes, metals)){
            ++result;
        }
        cout << result;
    }
}