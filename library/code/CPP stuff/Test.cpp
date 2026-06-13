#include <bits/stdc++.h>
#include <iomanip>
using namespace std;


float to_float(string x){
    float h = 0;
    int decimal = 0;
    bool switch1 = 0;
    for (int i = 0; i < int(x.length()); i++){
        if (x[i] == char('.')){
            switch1 = 1;
        }
        else{
            if (switch1){
                ++decimal;
                h = h + (x[i] - '0')/pow(10, decimal);
                cout << x[i] - '0' << '\t' << pow(10, decimal) << '\t' << h << endl;
            }
            else{
                h = h * 10 + (x[i] - '0');
            }
        }
    }
    return h;
}

int main(){
    int x = 0;
    string y;
    while (1){
        cin >> y;
        x = to_float(y);;
        cout << x << endl;
    }
}