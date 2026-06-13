#include<bits/stdc++.h>
using namespace std;

bool check_int(string x){
    for (int i = 0; i < int(x.length()); i++){
        if (isdigit(x[i]) == false){
            return false;
        }
    }
    return true;
}

int main(void){
    srand(time(0));
    int num = rand()%100 + 1;
    int limit = 0;
    string input = "";
    int stuff = 0;
    int low = 0;
    int high = 0;
    while(limit < 5 && stuff != num){
        cout << "Try a number (1-100): ";
        cin >> input;
        cout << endl;
        while (!check_int(input)){
            cout << "Try a int, ty: ";
            cin >> input;
            cout << endl;
        }
        stuff = 0;
        for (int i = 0; i < int(input.length()); i++){
            stuff = stuff * 10 + (input[i]);
        }
        if (stuff > 100 || stuff < 1){
            cout << stuff;
            cout << "The stupidity coming from your number is here. Try sth else (1-100)\n";
        }
        else{
            if (stuff > num){
                cout << "A bit too big, isn't it?\n";
            }
            else if (stuff < num){
                cout << "Too low...\n";
            }
            ++limit;
        }
    }
    if (limit == 5){
        cout << "gg";
    }
    else{
        cout << "noice";
    }
}