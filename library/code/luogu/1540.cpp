#include <bits/stdc++.h>
using namespace std;

int main(){
    int M, N, K;
    cin >> M >> N;
    queue<int> saved;
    vector<int> sorted(M);
    int result = 0;
    for (int k = 0; k < N; ++k){
        cin >> K;
        int i = 0;
        while (i < M && K <= sorted[i]){
            if (sorted[i] == K){
                break;
            }
            ++i;
        }
        if (sorted[i] != K){
            ++result;
            if (saved.size() < M){
                saved.push(K);
                sorted[0] = K;
            }
            else{
                for (auto j = sorted.begin(); j < sorted.end(); ++j){
                    if (*j == saved.front()){
                        sorted.erase(j);
                        saved.pop();
                        saved.push(K);
                        sorted[j - sorted.begin()] = K;
                    }
                }
                
            }
        }
        sort(sorted.begin(), sorted.end());
    }
    cout << result;
}
