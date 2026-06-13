#include <bits/stdc++.h>
using namespace std;

struct node {
    int sum, low;
};

node record[100010];
int record1[100010];

int main (){
    // freopen("testing.in", "r", stdin);
    freopen("homework.in", "r", stdin);
    freopen("homework.out", "w", stdout);
    int N, temp1, temp2 = INT_MAX;
    cin >> N;
    for (int i = 1; i <= N; ++i){
        cin >> temp1;
        record1[i] = temp1;
    }
    for (int i = N; i > 0; --i){
        record[i].sum = record[i + 1].sum + record1[i];
        temp2 = min(record1[i], temp2);
        record[i].low = temp2;
    }

    double score = 0, temp3;
    int result = 0;
    for (int i = 1; i < N; ++i){
        temp3 = (double) (record[i].sum - record[i].low)/ (double) (N - i);
        // cout << temp3 << endl;
        if (score < temp3){
            score = temp3;
            result = i - 1;
        }
    }
    cout << result;
}