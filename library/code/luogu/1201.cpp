#include <bits/stdc++.h>
using namespace std;

int main(){
    //freopen("testing.in", "r", stdin);
    int n;
    cin >> n;
    map<string, int> humans;
    string name;
    vector<string> names;
    for (int i = 0; i < n; ++i){
        cin >> name;
        humans[name]++;
        names.push_back(name);
    }
    int money, m;
    for (int i = 0; i < n; ++i){
        cin >> name >> money >> m;
        if (m != 0){
            humans[name] += money % m - money;
            money -= money % m;
            for (int j = 0; j < m; ++j){
                cin >> name;
                humans[name] += money / m;
            }
        }
    }
    
    for (int i = 0; i < n; ++i){
        cout << names[i] << " " << humans[names[i]] - 1 << endl;
    }
}