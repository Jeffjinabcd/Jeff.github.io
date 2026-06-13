#include <bits/stdc++.h>
using namespace std;

int num[1000001];
int t[1000001];

long long res;

void mergeSort(int left, int right) {
    if (left == right){
        return;
    }
    int mid = left + (right - left) / 2;
    mergeSort(left, mid);
    mergeSort(mid + 1, right);

    int k = left, i = left, j = mid + 1;
    while (i <= mid && j <= right){
        if (num[i] <= num[j]) {
            t[k] = num[i];
            ++i;
        }
        else if (num[i] > num[j]) {
            res += mid - i + 1;
            t[k] = num[j];
            ++j;
        }
        ++k;
    }

    while (i <= mid){
        t[k] = num[i];
        ++i;
        ++k;
    }

    while (j <= right){
        t[k] = num[j];
        ++j;
        ++k;
    }
    
    for (int m = left; m <= right; ++m){
        num[m] = t[m];
    }





}

int main() {
    int n;
    cin >> n;
    for (int i = 1; i <= n; i++){
        cin >> num[i];
    }

    mergeSort(1, n);

    cout << res;
    return 0;
}