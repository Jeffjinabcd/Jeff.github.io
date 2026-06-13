#include <bits/stdc++.h>
using namespace std;

int L, N, M;
vector<int> record (50001);

bool check (int m){
    int previous = 0;
    int current = 1;
    int remove = 0;
    while (current < N + 2){
        if ((record[current] - record[previous]) < m){
            ++remove;
            ++current;
        }
        else{
            previous = current;
            ++current;
        }
    }
    return remove <= M;
}

int main (){
    // freopen("testing.in", "r", stdin);
    cin >> L >> N >> M;
    record[0] = 0;
    for (int i = 1; i <= N; ++i){
        cin >> record[i];
    }
    record[N + 1] = L;
    int left = 1, right = L;
    int ans = 0;
    while (left <= right){
        int mid = left + (right - left) / 2;
        // acout << left << "\t" << mid << "\t" << right << endl;
        if (check(mid)){
            left = mid + 1;
            ans = mid;
        }
        else {
            right = mid - 1;
        }
    }
    cout << ans;
}