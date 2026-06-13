#include <bits/stdc++.h>
using namespace std;


int num[1000001];

void quickSort(int left, int right)
{
    if (left < right) {
        int i = left, j = right;
        int pivot = num[(left + right) / 2];
        while (left <= right){
            while (num[left] < pivot) {
                ++left;
            }
            while (num[right] > pivot) {
                --right;
            }
            if (left <= right){
                swap(num[left], num[right]);
                left++;
                right--;
            }
        }
        quickSort(i, right);
        quickSort(left, j);
    }
}


int main() {
    int n;
    cin >> n;
    for (int i = 1; i <= n; i++){
        cin >> num[i];
    }
    random_shuffle(num + 1, num + 1 + n);
    quickSort(1, n);

    for (int i = 1; i <= n; i++){
        cout << num[i] << " ";
    }
    return 0;
}