#include<bits/stdc++.h>
using namespace std;
int a[15], n, s;
bool b[100], c[100], d[100];

void print(){
    ++s;
    if(s <= 3){
        for(int i = 1; i <= n; i++){
            cout << a[i] << " ";
        }
        cout << endl;
    }
}
void search(int i){
    for(int j = 1; j <= n; j++){
        if(!b[j] && !c[i + j] && !d[i - j + n]){
            a[i] = j;
            b[j] = true;
            c[i + j] = true;
            d[i - j + n] = true;
            if(i == n) {
                print();
            }
            else{
                search(i + 1);
            }
            b[j] = false;
            c[i + j] = false;
            d[i - j + n] = false;
        }
    }
}
int main(){
    cin >> n;
    search(1); 
    cout << s << endl;
    return 0;
}