#include <bits/stdc++.h>
using namespace std;
int data[101];

/**
二分查找
样例1
input:
10
1 3 4 5 7 8 9 10 20 21
11
output:
not found

样例2
input:
10
1 3 4 5 7 8 9 10 20 21
9
output:
7
 */
int main()
{
    int n;
    cin>>n;
    for(int i=0; i<n; i++) {
        cin>>data[i];
    }
    sort(data, data + n);
    int target;
    cin>>target;
    int left = 0;
    int right = n-1;
    while(left <= right) {
        int mid = (left + right) / 2;  // 防止溢出写法 left + (right-left)/2
        if(data[mid] == target) {
            cout << mid + 1;
            return 0;
        }
        if(data[mid] > target) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    cout<<"not found"<<endl;
    return 0;
}
