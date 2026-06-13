#include <bits/stdc++.h>
using namespace std;


long long record[50][50][50];

long long W(long long a, long long b, long long c){
    if (a <= 0 || b <= 0 || c <= 0){
        return 1;
    }
    else if (a > 20 || b > 20 || c > 20){
        if (record[20][20][20] == 0){
            record[20][20][20] == W(20, 20, 20);
        }
        return record[20][20][20];
    }
    else if (a < b && b < c){
        if (record[a][b][c - 1] == 0){
            record[a][b][c - 1] = W(a, b, c - 1);
        }
        if (record[a][b - 1][c - 1] == 0){
            record[a][b - 1][c - 1] = W(a, b - 1, c - 1);
        }
        if (record[a][b - 1][c] == 0){
            record[a][b - 1][c] = W(a, b - 1, c);
        }
        record[a][b][c] = record[a][b][c - 1] + record[a][b - 1][c - 1] - record[a][b - 1][c];
    }
    else {
        if (record[a - 1][b][c] == 0){
            record[a - 1][b][c] = W(a - 1, b, c);
        }
        if (record[a - 1][b - 1][c] == 0){
            record[a - 1][b - 1][c] = W(a - 1, b - 1, c);
        }
        if (record[a - 1][b][c - 1] == 0){
            record[a - 1][b][c - 1] = W(a - 1, b, c - 1);
        }
        if (record[a - 1][b - 1][c - 1] == 0){
            record[a - 1][b - 1][c - 1] = W(a - 1, b - 1, c - 1);
        }
        record[a][b][c] = record[a - 1][b][c] + record[a - 1][b - 1][c] + record[a - 1][b][c - 1] - record[a - 1][b - 1][c - 1];
        
    }
    return record[a][b][c];
}

int main () {
    // freopen("testing.in", "r", stdin);
    long long temp1, temp2, temp3;
    while (cin >> temp1 >> temp2 >> temp3) {
        if ((temp1 == -1 && temp2 == -1 && temp3 == -1)){
            return 0;
        }
        cout << "w(" << temp1 << ", " << temp2 << ", " << temp3 << ") = " << W(temp1, temp2, temp3) << endl;
    }
}