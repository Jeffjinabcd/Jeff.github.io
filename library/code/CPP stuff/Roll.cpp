#include<bits/stdc++.h>
using namespace std;

int main(){
    srand(time(0));
    char x[4][100] = {"Jefferson", "Jeff", "Leon", "Haihan"};
    int n = 0;
    char y = ' ';
    cout << "insert x if wanna quit, otherwise, insert something for it to work(One character pls and ty)";
    while (y != char('x')){
        cin >> y;
        n = rand() % 4;
        cout << x[n] << "\n";
    }
}