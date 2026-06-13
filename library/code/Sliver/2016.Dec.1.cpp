#include <bits/stdc++.h>
using namespace std;

long long record[100002];

int N, Q;

int search(int start, int end){
    if (start == end){
        return 0;
    }
    int i, j;
    
    int left = 0, right = 100001;
    while (left < right){
        int mid = left + (right - left) / 2;
        if (record[mid] < start){
            left = mid + 1;
        }
        else {
            right = mid;
        }
    }
    i = left;
    right = 100001;
    while (left < right){
        int mid = left + (right - left) / 2;
        if (record[mid] < end){
            left = mid + 1;
        }
        else {
            right = mid;
        }
    }
    j = left;
    int res = j - i;
    if (record[j] == end){
        ++res;
    }
    return res;
}

int main () {
    // freopen("testing.in", "r", stdin);
    freopen("haybales.in", "r", stdin);
    freopen("haybales.out", "w", stdout);
    cin >> N >> Q;
    record[100001] = INT_MAX;
    for (int i = 1; i <= N; ++i){
        int temp;
        cin >> temp;
        record[i] = temp;
    }
    sort(record, record + 100002);
    for (int i = 0; i < Q; ++i){
        int temp1, temp2;
        cin >> temp1 >> temp2;
        cout << search(temp1, temp2) << endl;
    }
}