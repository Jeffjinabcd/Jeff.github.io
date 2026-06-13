#include <bits/stdc++.h>
using namespace std;

list<int> mylist;

int main()
{
    list<int>::iterator itr;

    // set some initial values:
    for (int i=1; i<=5; i++) mylist.push_back(i); // 1 2 3 4 5

    itr = mylist.begin();
    ++itr;       // it points now to number 2

    mylist.insert (itr,10);                        // 1 10 2 3 4 5

    // "it" still points to number 2                      
    mylist.insert (itr,2,20);                      // 1 10 20 20 2 3 4 5

    --itr;       // it points now to the second 20            

    vector<int> myvector (2,30);
    mylist.insert (itr,myvector.begin(),myvector.end());
                                                // 1 10 20 30 30 20 2 3 4 5
    // mylist contains: 100 20 30 30 20 2 3 4 5
    cout << "mylist contains:";
    mylist.erase(mylist.begin());
    itr = mylist.begin();
    *itr = 100;  // update
    for (; itr!=mylist.end(); itr++)
        cout << " " << *itr;
    cout << endl;


    return 0;
}

