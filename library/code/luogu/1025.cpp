#include <bits/stdc++.h>
using namespace std;
#define LL long long

int N, K;

LL res;

void DFS(int count, int sum, int last){
    if (sum == N && count == K){
        ++res;
        return;
    }
    if (sum + last > N || count >= K){
        return;
    }
    ++count;
    for (int i = last; i <= N - sum; ++i){
        DFS(count, sum + i, i);
    }
    return;
}

int main () {
    cin >> N >> K;
    DFS(0, 0, 1);
    cout << res;
}