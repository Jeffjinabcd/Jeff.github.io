#include <bits/stdc++.h>
using namespace std;

vector<int> num;

int main(){
    string s;
    cin >> s;
    for(int i = 0; i < s.length(); i++){
        num.push_back(s[i] - '0');
    }
    int k;
    cin >> k;
    for(int i = 0; i < k; i++){
        int j = 0;
        while(j < num.size() - 1 && num[j] <= num[j+1]){
            j++;
        }
        num.erase(num.begin() + j);
    }

    bool nonzero = false;
    for(int i=0; i< num.size() - 1; i++) {
        if(num[i] > 0 ){
            nonzero = true;
        }
        if (nonzero) {
            cout << num[i];
        }
    }
    cout << num[num.size() - 1];

    return 0;
}

