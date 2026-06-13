#include <bits/stdc++.h>
using namespace std;

char intToAlphabet( int i ) {
  return static_cast<char>('A' - 1 + i);
}

int main (){
    map<char, int> dic;
    for (int i = 0; i <= 26; ++i){
        dic[intToAlphabet(i)] += i;
    }
    int N, Q;
    char str[N];
    cin >> N >> Q, str;
    for (int i = 1; i <= N; ++i){
        cin >> str[i];
    }
    char current = intToAlphabet(0);
    int prefix[N];
    int suffix[N];
    int s, e;
    for (int i = 1; i <= N; ++i){
        prefix[i] = prefix[i - 1];
        if (dic[current] < dic[str[i]]){
            
        }
    }

    for (int i = 0; i < Q; ++i){
        cin >> s >> e;
    }
}