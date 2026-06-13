#include<bits/stdc++.h>
#include<windows.h>
#include<iomanip>
using namespace std;

void dash(string y = "") {
    cout << "_____";
    cout << y;
    for (int i = 0; i < 20 - y.length(); ++i) {
        cout << "_";
    }
    cout << endl;
}

void text_color(string x) {
    /*
    1  : Blue
    2  : Green
    3  : Cyan
    4  : Red
    5  : Purple
    6  : Yellow(dark)
    7  : Default white
    8  : Grey
    9  : Bright blue
    10 : Bright green
    11 : Bright cyan
    12 : Bright red
    13 : pink
    14 : yellow
    15 : Bright white
    */
    if (x == "Blue") {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
    }
    else if (x == "Green") {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
    }
    else if (x == "Cyan") {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
    }
    else if (x == "Red") {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
    }
    else if (x == "Purple") {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5);
    }
    else if (x == "Dark Yellow") {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
    }
    else if (x == "White") {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    }
    else if (x == "Grey") {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
    }
    else if (x == "Bright Blue") {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
    }
    else if (x == "Bright Green") {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
    }
    else if (x == "Bright Cyan") {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    }
    else if (x == "Bright Red") {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
    }
    else if (x == "Pink") {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
    }
    else if (x == "Yellow") {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    }
    else if (x == "Bright White") {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    }
}

class Node {
    private:
        float Num;
        Node* Left;
        Node* Right;
        int Height;
    public:
        Node(float _Num = 0, Node* _Left = nullptr, Node* _Right = nullptr, int _Height = 1);
        float GetNum();
        Node* GetLeft();
        Node* GetRight();
        int GetHeight();
        void SetNum(float);
        void SetLeft(Node*);
        void SetRight(Node*);
        void SetLR(Node*);
        void SetHeight(int);
        void ChangeHeight();
        void Print();
};

Node::Node(float _Num, Node* _Left, Node* _Right, int _Height) {
    Num = _Num;
    Left = _Left;
    Right = _Right;
    Height = _Height;
}

float Node::GetNum() {
    return Num;
}

Node* Node::GetLeft() {
    return Left;
}

Node* Node::GetRight() {
    return Right;
}

int Node::GetHeight() {
    return Height;
}

void Node::SetNum(float _Num) {
    Num = _Num;
}

void Node::SetLeft(Node* _Left) {
    Left = _Left;
}

void Node::SetRight(Node* _Right) {
    Right = _Right;
}

void Node::SetLR(Node* Depends) {
    if (Num > Depends->GetNum()) {
        SetLeft(Depends);
    }
    else {
        SetRight(Depends);
    }
}

void Node::SetHeight(int _Height) {
    Height = _Height;
}

void Node::ChangeHeight() {
    int ls = 0, rs = 0;
    if (GetLeft() != nullptr) {
        ls = GetLeft()->GetHeight();
    }
    if (GetRight() != nullptr) {
        rs = GetRight()->GetHeight();
    }

    SetHeight(1 + max(ls, rs));
}

void Node::Print() {
    cout << Num << "\t" << Height << endl;
    cout << Left << "\t";
    if (Left != nullptr) {
        cout << Left->GetNum() << endl;
    }
    else {
        cout << endl;
    }
    cout << Right << "\t";
    if (Right != nullptr) {
        cout << Right->GetNum() << endl;
    }
    else {
        cout << endl;
    }
    cout << endl;
}

class Binary {
    private:
        Node* head;
        int Length;
    public:
        Binary(float _first = 0);
        int GetDifference(Node*);
        Node* Gethead();
        int GetLength();
        Node* rotateRight(Node*);
        Node* rotateLeft(Node*);
        void insert(float);
        void erase(float);
        Node* find(float);
        void print(Node*);
        void printmm(Node*);
};

Binary::Binary(float _first) {
    if (_first != 0) {
        head = new Node(_first);
        Length = 1;
    }
    else {
        head = nullptr;
        Length = 0;
    }
}

int Binary::GetDifference(Node* node) {
    if (node == nullptr) {
        return 0;
    }
    if (node->GetLeft() != nullptr) {
        if (node->GetRight() != nullptr) {
            return node->GetLeft()->GetHeight() - node->GetRight()->GetHeight();
        }
        else {
            return node->GetLeft()->GetHeight();
        }
    }
    else {
        if (node->GetRight() != nullptr) {
            return - node->GetRight()->GetHeight();
        }
        else {
            return 0;
        }
    }
}

Node* Binary::Gethead() {
    return head;
}

int Binary::GetLength() {
    return Length;
}

Node* Binary::rotateRight(Node* top) {
    Node* l = top->GetLeft();
    Node* lr = l->GetRight();
    
    l->SetRight(top);
    top->SetLeft(lr);

    int ls = 0, rs = 0;
    if (top->GetLeft() != nullptr) {
        ls = top->GetLeft()->GetHeight();
    }
    if (top->GetRight() != nullptr) {
        rs = top->GetRight()->GetHeight();
    }

    top->SetHeight(1 + max(ls, rs));

    ls = 0, rs = 0;
    if (l->GetLeft() != nullptr) {
        ls = l->GetLeft()->GetHeight();
    }
    if (l->GetRight() != nullptr) {
        rs = l->GetRight()->GetHeight();
    }

    l->SetHeight(1 + max(ls, rs));

    return l;
}

Node* Binary::rotateLeft(Node* top) {
    Node* r = top->GetRight();
    Node* rl = r->GetLeft();

    r->SetLeft(top);
    top->SetRight(rl);

    int ls = 0, rs = 0;
    if (top->GetLeft() != nullptr) {
        ls = top->GetLeft()->GetHeight();
    }
    if (top->GetRight() != nullptr) {
        rs = top->GetRight()->GetHeight();
    }

    top->SetHeight(1 + max(ls, rs));

    ls = 0, rs = 0;
    if (r->GetLeft() != nullptr) {
        ls = r->GetLeft()->GetHeight();
    }
    if (r->GetRight() != nullptr) {
        rs = r->GetRight()->GetHeight();
    }

    r->SetHeight(1 + max(ls, rs));

    return r;
}

void Binary::insert(float x) {
    if (Length == 0) {
        head = new Node(x);
        ++Length;
    }
    else {
        Node* temp = head;
        vector <Node*> path;
        bool switch1 = 1;
        while (temp != nullptr) {
            if (temp->GetNum() > x) {
                path.push_back(temp);
                temp = temp->GetLeft();
            }
            else if (temp->GetNum() < x) {
                path.push_back(temp);
                temp = temp->GetRight();
            }
            else {
                switch1 = 0;
                temp = nullptr;
            }
        }
        if (switch1) {
            Node* mie = new Node (x);
            if (path.back()->GetNum() > x) {
                path.back()->SetLeft(mie);
            }
            else {
                path.back()->SetRight(mie);
            }
            for (int i = path.size() - 1; i > 0; --i) {
                temp = path[i];
                if (GetDifference(temp) == 2) {
                    if (temp->GetLeft()->GetNum() > x) {
                        (path[i - 1])->SetLR(rotateRight(temp));
                    }
                    else {
                        temp->SetLeft(rotateLeft(temp->GetLeft()));
                        (path[i - 1])->SetLR(rotateRight(temp));
                    }
                }
                else if (GetDifference(temp) == -2) {
                    if (temp->GetRight()->GetNum() < x) {
                        (path[i - 1])->SetLR(rotateLeft(temp));
                    }
                    else {
                        temp->SetRight(rotateRight(temp->GetRight()));
                        (path[i - 1])->SetLR(rotateLeft(temp));
                    }
                }
                else {
                    temp->ChangeHeight();
                }
            }
            if (GetDifference(head) == 2) {
                if (head->GetLeft()->GetNum() > x) {
                    head = rotateRight(head);
                }
                else {
                    head->SetLeft(rotateLeft(head->GetLeft()));
                    head = rotateRight(head);
                }
            }
            else if (GetDifference(head) == -2) {
                if (head->GetRight()->GetNum() < x) {
                    head = rotateLeft(head);
                }
                else {
                    head->SetRight(rotateRight(head->GetRight()));
                    head = rotateLeft(head);
                }
            }
            else {
                head->ChangeHeight();
            }
            ++Length;
        }
        else {
            cout << endl;
            dash("Exist");
            cout << endl;
        }
    }
}

void Binary::erase(float x) {
    if (Length == 0) {
        cout << "Empty";
    }
    else {
        vector <Node*> path;
        vector <Node*> change_path;
        Node* temp = head;
        bool switch1 = 1;
        while (switch1 && temp != nullptr) {
            if (temp->GetNum() > x) {
                path.push_back(temp);
                change_path.push_back(temp);
                temp = temp->GetLeft();
            }
            else if (temp->GetNum() < x) {
                path.push_back(temp);
                change_path.push_back(temp);
                temp = temp->GetRight();
            }
            else {
                switch1 = 0;
            }
        }
        if (temp == nullptr) {
            cout << "DNE";
        }
        else {
            if (path.size() > 0) {
                if (temp->GetHeight() == 1) {
                    if (temp->GetNum() > path.back()->GetNum()) {
                        path.back()->SetRight(nullptr);
                    }
                    else {
                        path.back()->SetLeft(nullptr);
                    }
                }
                else if (temp->GetLeft() == nullptr) {
                    if (temp->GetNum() > path.back()->GetNum()) {
                        path.back()->SetRight(temp->GetRight());
                    }
                    else {
                        path.back()->SetLeft(temp->GetRight());
                    }
                }
                else if (temp->GetRight() == nullptr) {
                    if (temp->GetNum() > path.back()->GetNum()) {
                        path.back()->SetRight(temp->GetLeft());
                    }
                    else {
                        path.back()->SetLeft(temp->GetLeft());
                    }
                }
                else {
                    if (GetDifference(temp) >= 0) {
                        Node* sub_mini = temp->GetLeft();
                        vector <Node*> mini_path;
                        mini_path.push_back(temp);
                        change_path.push_back(temp);
                        while (sub_mini != nullptr) {
                            mini_path.push_back(sub_mini);
                            change_path.push_back(sub_mini);
                            sub_mini = sub_mini->GetRight();
                        }
                        temp->SetNum(mini_path.back()->GetNum());
                        if (mini_path.size() == 2) {
                            mini_path[mini_path.size() - 2]->SetLeft(mini_path.back()->GetLeft());
                        }
                        else {
                            mini_path[mini_path.size() - 2]->SetRight(mini_path.back()->GetLeft());
                        }
                        change_path.pop_back();
                    }
                    else {
                        Node* sub_mini = temp->GetRight();
                        vector <Node*> mini_path;
                        mini_path.push_back(temp);
                        change_path.push_back(temp);
                        while (sub_mini != nullptr) {
                            mini_path.push_back(sub_mini);
                            change_path.push_back(sub_mini);
                            sub_mini = sub_mini->GetLeft();
                        }
                        temp->SetNum(mini_path.back()->GetNum());
                        if (mini_path.size() == 2) {
                            mini_path[mini_path.size() - 2]->SetRight(mini_path.back()->GetRight());
                        }
                        else {
                            mini_path[mini_path.size() - 2]->SetLeft(mini_path.back()->GetRight());
                        }
                        change_path.pop_back();
                    }
                }
            }
            else {
                if (Length == 1){
                    head = nullptr;
                }
                else if (head->GetLeft() == nullptr) {
                    head = head->GetRight();
                }
                else if (head->GetRight() == nullptr) {
                    head = head->GetLeft();
                }
                else if (GetDifference(head) >= 0) {
                    Node* sub_mini = head->GetLeft();
                    vector <Node*> mini_path;
                    mini_path.push_back(head);
                    change_path.push_back(head);
                    while (sub_mini != nullptr) {
                        mini_path.push_back(sub_mini);
                        change_path.push_back(sub_mini);
                        sub_mini = sub_mini->GetRight();
                    }
                    head->SetNum(mini_path.back()->GetNum());
                    if (mini_path.size() == 2) {
                        mini_path[mini_path.size() - 2]->SetLeft(mini_path.back()->GetLeft());
                    }
                    else {
                        mini_path[mini_path.size() - 2]->SetRight(mini_path.back()->GetLeft());
                    }
                    change_path.pop_back();
                }
                else {
                    Node* sub_mini = head->GetRight();
                    vector <Node*> mini_path;
                    mini_path.push_back(head);
                    change_path.push_back(head);
                    while (sub_mini != nullptr) {
                        mini_path.push_back(sub_mini);
                        change_path.push_back(sub_mini);
                        sub_mini = sub_mini->GetLeft();
                    }
                    head->SetNum(mini_path.back()->GetNum());
                    if (mini_path.size() == 2) {
                        mini_path[mini_path.size() - 2]->SetRight(mini_path.back()->GetRight());
                    }
                    else {
                        mini_path[mini_path.size() - 2]->SetLeft(mini_path.back()->GetRight());
                    }
                    change_path.pop_back();
                }
            }
            for (int i = change_path.size() - 1; i >= 0; --i) {
                temp = change_path[i];
                if (GetDifference(temp) == 2) {
                    if (GetDifference(temp->GetLeft()) == -1) {
                        temp->SetLeft(rotateLeft(temp->GetLeft()));
                        (change_path[i - 1])->SetLR(rotateRight(temp));
                    }
                    else {
                        (change_path[i - 1])->SetLR(rotateRight(temp));
                    }
                }
                else if (GetDifference(temp) == -2) {
                    if (GetDifference(temp->GetRight()) == 1) {
                        temp->SetRight(rotateRight(temp->GetRight()));
                        (change_path[i - 1])->SetLR(rotateLeft(temp));
                    }
                    else {
                        (change_path[i - 1])->SetLR(rotateLeft(temp));
                    }
                }
                else {
                    temp->ChangeHeight();
                }
            }
            if (GetDifference(head) == 2) {
                if (GetDifference(head->GetLeft()) == -1) {
                    head->SetLeft(rotateLeft(head->GetLeft()));
                    head = rotateRight(head);
                }
                else {
                    head = rotateRight(head);
                }
            }
            else if (GetDifference(head) == -2) {
                if (GetDifference(temp->GetRight()) == 1) {
                    head->SetRight(rotateRight(head->GetRight()));
                    head = rotateLeft(head);
                }
                else {
                    head = rotateLeft(head);
                }
            }
            if (head != nullptr) {
                head->ChangeHeight();
            }
            --Length;
        }
    }
}

Node* Binary::find(float x) {
    Node* temp = head;
    while (temp != nullptr && temp->GetNum() != x){
        if (temp->GetNum() > x) {
            temp = temp->GetLeft();
        }
        else if (temp->GetNum() < x) {
            temp = temp->GetRight();
        }
    }
    return temp;
}

void Binary::print(Node* x) {
    if (Length == 0) {
        cout << "Empty";
    }
    else {
        if (x->GetLeft() != nullptr) {
            print(x->GetLeft());
        }
        x->Print();
        cout << "Difference: " << GetDifference(x) << endl;

        if (x->GetRight() != nullptr) {
            print(x->GetRight());
        }
    }
}

void Binary::printmm(Node* x) {
    if (Length == 0) {
        cout << "Empty";
    }
    else {
        x->Print();
        if (x->GetLeft() != nullptr) {
            x = x->GetLeft();
        }
        if (x->GetRight() != nullptr) {
            x = x->GetRight();
        }
        cout << endl;
    }
}

int main() {
    Binary a;
    float c;
    char d;
    bool switch1 = 1;
    while (switch1) {
        cin >> d;
        if (d == char('+')) {
            cin >> c;
            cout << c << "\tinsert..." << endl;
            a.insert(c);
        }
        else if (d == char('-')) {
            cin >> c;
            cout << c << "\terase..." << endl;
            a.erase(c);
        }
        else if (d == char('@')) {
            cin >> c;
            cout << c << "\tfind..." << endl;
            Node* finddd = a.find(c);
            if (finddd != nullptr){
                finddd->Print();
            }
            else {
                cout << "DNE" << endl;
            }
        }
        else if (d == char('p')) {
            cout << "\tprint..." << endl;
            a.print(a.Gethead());
            cout << "Length: " << a.GetLength() << endl;
        }
        else if (d == char('x')) {
            cout << "dead..." << endl;
            switch1 = 0;
        }
        dash();
        //a.print(a.Gethead());
        dash();
    }
    dash();
    
}

/*
+  919
-  919
+  696
+  974
+  944
-  944
+  797
+  511
-  176
-  388
+  421
+  650
-  696
+  369
+  412
+  245
+  131
+  124
+  907
+  431
-  907
-  369
-  124
-  974
-  431
+  33
+  982
-  151
+  508
-  757
-  650
+  301
-  301
+  316
-  316
-  421
+  665
+  349
+  474
+  74
-  474
-  459
+  390
+  10
+  130
-  508
+  307
-  515
+  556
+  156
+  758
+  863
+  450
+  153
+  510
+  282
+  868
+  263
-  412
-  263
-  675
+  736
+  928
+  379
+  672
+  791
+  720
-  450
+  785
+  437
-  556
+  119
-  785
+  897
-  104
+  248
+  376
+  279
-  74
-  531
+  482
-  510
+  558
+  542
+  372
-  33
+  227
-  248
-  720
+  834
+  449
+  960
+  100
-  758
+  261
+  854
+  63
+  512
+  356
-  854

*/
/*
+  1518
+  47
-  1518
+  4460
+  4768
+  3484
+  636
+  4021
-  636
+  2482
-  2482
+  1374
+  267
+  2107
+  2657
+  549
+  2037
+  1947
-  549
+  3976
-  1947
+  2992
+  3654
+  3185
+  3434
-  3484
-  2905
-  3654
+  3636
+  3215
+  4221
-  4121
+  2560
+  1552
-  2992
+  1574
+  684
-  267
-  2107
+  4614
+  1800
-  1333
+  1980
+  105
+  794
+  3945
+  3787
-  3976
+  892
+  4188
+  4453
+  3209
+  4425
-  3945
-  2298
+  2625
+  3688
+  3954
-  912
-  3954
+  4926
+  541
-  1482
+  1460
+  2594
+  1719
+  4725
-  1980
+  3222
-  4221
-  3209
+  3427
+  2002
+  659
+  152
+  2710
-  794
+  873
+  1211
+  4607
+  3336
-  2657
+  2467
+  3302
+  4790
+  1017
+  2036
-  2782
+  3234
-  541
-  4544
+  4713
+  68
+  3553
+  2319
+  2945
-  2002
-  2704
+  356
+  1644
+  4292
+  3775
-  4790
+  3987
+  1781
+  4760
+  3444
+  1807
-  1460
+  729
+  1790
-  2945
-  2625
+  1230
+  4880
+  1405
+  2169
+  311
+  4947
-  2710
+  529
+  1321
+  1744
+  1431
-  4880
+  161
+  979
+  482
-  652
+  334
-  3553
-  873
-  659
-  1017
+  79
+  4482
+  3423
+  202
+  2287
+  3152
-  977
+  39
+  912
+  4227
+  1765
+  4802
+  3984
+  4778
+  2355
+  287
+  263
-  4585
-  4725
+  3733
+  2564
+  1220
-  987
+  2034
+  626
+  4187
+  2615
+  4465
-  1728
-  79
+  1971
+  3014
+  3617
+  3755
+  857
+  3879
-  4425
+  2889
-  4755
+  3365
-  2319
+  1242
-  1619
+  4495
+  4932
+  3686
+  4773
+  4507
+  4010
-  1830
-  4495
+  2057
-  3427
+  1786
+  3421
+  141
+  23
-  3759
+  706
+  523
+  4223
-  4507
-  334
+  1823
+  2381
-  1744
+  3218
+  1576
+  3092
+  597
-  2054
-  933
-  2287
+  4190
-  1800
+  2521
+  4681
+  43
+  1149
-  706
+  2215
-  529
-  1781
+  359
-  43
+  634
+  636
-  2560
+  3344
-  3700
+  2292
-  3365
+  1760
-  3215
+  2139
+  2763
-  4614
+  3028
+  2946
+  4957
-  2292
+  1629
-  1491
+  3922
+  624
-  1431
+  4937
+  1528
+  3177
+  4094
+  4626
+  257
+  2464
+  4624
+  1187
-  1765
+  2168
-  2681
+  1701
+  4243
+  265
+  4531
+  3439
+  2228
-  2215
+  3816
+  3072
+  1824
-  3641
+  1444
-  4482
+  242
+  4766
-  626
+  481
-  3421
-  3344
+  2699
+  4743
-  2835
+  340
+  1210
+  3260
+  645
-  3617
-  287
-  4937
-  4460
+  3593
-  2168
+  352
+  295
-  4766
+  1191
+  1332
+  3921
+  574
+  3466
+  869
-  4778
+  2755
+  214
+  566
-  523
-  1922
-  4802
+  3143
-  4292
+  418
+  2891
-  3987
-  2537
+  510
-  4188
+  3710
+  2115
+  4850
+  4503
-  4474
-  857
-  3775
-  161
+  2525
+  2096
+  3515
-  2763
+  1585
+  732
+  3061
-  1745
+  2670
-  669
-  3686
-  2464
+  1826
+  277
+  3132
+  4389
-  263
+  3049
+  1658
+  3161
+  3209
+  591
-  1187
+  1477
+  78
+  4172
-  2594
-  3733
-  1405
-  3286
+  2833
+  2784
-  3466
+  167
-  2564
+  36
+  1197
+  1707
+  299
-  1629
+  2911
-  1701
+  1357
+  2690
+  896
-  1971
-  3128
+  210
-  4531
+  3414
-  4947
-  1824
-  311
+  4199
+  4483
-  2755
-  295
+  233
-  1126
+  1245
+  763
-  1197
-  3177
+  962
+  690
-  2381
+  2285
+  156
+  3751
-  167
+  781
-  1786
+  4685
+  4425
+  61
-  4187
+  3798
+  4538
+  1888
+  236
-  4713
-  1242
+  4037
+  3210
+  454
-  892
-  4094
+  2578
+  3783
-  912
+  2491
+  398
+  4644
+  2512
+  1962
+  4489
+  843
-  3673
+  282
+  3077
+  3633
+  2645
-  2912
-  126
+  4092
-  4607
+  2896
-  3222
+  4730
+  4067
+  4652
-  159
+  4795
-  4067
-  1823
-  1263
+  3810
-  2512
+  3316
+  4166
+  4699
+  4152
+  3141
-  1210
+  2668
+  1129
+  434
-  299
-  2911
-  2491
-  4624
+  1728
-  3633
-  4465
-  2405
-  3781
+  809
+  1259
-  1775
+  1024
+  1721
+  1693
+  3950
-  3414
+  4323
+  537
-  1129
+  3566
-  61
+  3031
+  3875
-  2990
+  1781
+  2840
+  3042
-  2615
+  4921
+  2250
+  2764
-  3621
-  1585
+  3900
+  1176
-  3092
-  2525
+  665
+  1063
+  3298
+  1306
+  2460
+  3245
+  2257
-  3816
+  4119
+  1337
-  434
-  1245
+  3803
+  1410
+  4561
+  86
+  960
-  3260
+  372
*/

/*
+  1518
+  47
+  1518
+  4460
+  4768
+  3484
+  636
+  4021
+  636
+  2482
+  2482
+  1374
+  267
+  2107
+  2657
+  549
+  2037
+  1947
+  549
+  3976
+  1947
+  2992
+  3654
+  3185
+  3434
+  3484
+  2905
+  3654
+  3636
+  3215
+  4221
+  4121
+  2560
+  1552
+  2992
+  1574
+  684
+  267
+  2107
+  4614
+  1800
+  1333
+  1980
+  105
+  794
+  3945
+  3787
+  3976
+  892
+  4188
+  4453
+  3209
+  4425
+  3945
+  2298
+  2625
+  3688
+  3954
+  912
+  3954
+  4926
+  541
+  1482
+  1460
+  2594
+  1719
+  4725
+  1980
+  3222
+  4221
+  3209
+  3427
+  2002
+  659
+  152
+  2710
+  794
+  873
+  1211
+  4607
+  3336
+  2657
+  2467
+  3302
+  4790
+  1017
+  2036
+  2782
+  3234
+  541
+  4544
+  4713
+  68
+  3553
+  2319
+  2945
+  2002
+  2704
+  356
+  1644
+  4292
+  3775
+  4790
+  3987
+  1781
+  4760
+  3444
+  1807
+  1460
+  729
+  1790
+  2945
+  2625
+  1230
+  4880
+  1405
+  2169
+  311
+  4947
+  2710
+  529
+  1321
+  1744
+  1431
+  4880
+  161
+  979
+  482
+  652
+  334
+  3553
+  873
+  659
+  1017
+  79
+  4482
+  3423
+  202
+  2287
+  3152
+  977
+  39
+  912
+  4227
+  1765
+  4802
+  3984
+  4778
+  2355
+  287
+  263
+  4585
+  4725
+  3733
+  2564
+  1220
+  987
+  2034
+  626
+  4187
+  2615
+  4465
+  1728
+  79
+  1971
+  3014
+  3617
+  3755
+  857
+  3879
+  4425
+  2889
+  4755
+  3365
+  2319
+  1242
+  1619
+  4495
+  4932
+  3686
+  4773
+  4507
+  4010
+  1830
+  4495
+  2057
+  3427
+  1786
+  3421
+  141
+  23
+  3759
+  706
+  523
+  4223
+  4507
+  334
+  1823
+  2381
+  1744
+  3218
+  1576
+  3092
+  597
+  2054
+  933
+  2287
+  4190
+  1800
+  2521
+  4681
+  43
+  1149
+  706
+  2215
+  529
+  1781
+  359
+  43
+  634
+  636
+  2560
+  3344
+  3700
+  2292
+  3365
+  1760
+  3215
+  2139
+  2763
+  4614
+  3028
+  2946
+  4957
+  2292
+  1629
+  1491
+  3922
+  624
+  1431
+  4937
+  1528
+  3177
+  4094
+  4626
+  257
+  2464
+  4624
+  1187
+  1765
+  2168
+  2681
+  1701
+  4243
+  265
+  4531
+  3439
+  2228
+  2215
+  3816
+  3072
+  1824
+  3641
+  1444
+  4482
+  242
+  4766
+  626
+  481
+  3421
+  3344
+  2699
+  4743
+  2835
+  340
+  1210
+  3260
+  645
+  3617
+  287
+  4937
+  4460
+  3593
+  2168
+  352
+  295
+  4766
+  1191
+  1332
+  3921
+  574
+  3466
+  869
+  4778
+  2755
+  214
+  566
+  523
+  1922
+  4802
+  3143
+  4292
+  418
+  2891
+  3987
+  2537
+  510
+  4188
+  3710
+  2115
+  4850
+  4503
+  4474
+  857
+  3775
+  161
+  2525
+  2096
+  3515
+  2763
+  1585
+  732
+  3061
+  1745
+  2670
+  669
+  3686
+  2464
+  1826
+  277
+  3132
+  4389
+  263
+  3049
+  1658
+  3161
+  3209
+  591
+  1187
+  1477
+  78
+  4172
+  2594
+  3733
+  1405
+  3286
+  2833
+  2784
+  3466
+  167
+  2564
+  36
+  1197
+  1707
+  299
+  1629
+  2911
+  1701
+  1357
+  2690
+  896
+  1971
+  3128
+  210
+  4531
+  3414
+  4947
+  1824
+  311
+  4199
+  4483
+  2755
+  295
+  233
+  1126
+  1245
+  763
+  1197
+  3177
+  962
+  690
+  2381
+  2285
+  156
+  3751
+  167
+  781
+  1786
+  4685
+  4425
+  61
+  4187
+  3798
+  4538
+  1888
+  236
+  4713
+  1242
+  4037
+  3210
+  454
+  892
+  4094
+  2578
+  3783
+  912
+  2491
+  398
+  4644
+  2512
+  1962
+  4489
+  843
+  3673
+  282
+  3077
+  3633
+  2645
+  2912
+  126
+  4092
+  4607
+  2896
+  3222
+  4730
+  4067
+  4652
+  159
+  4795
+  4067
+  1823
+  1263
+  3810
+  2512
+  3316
+  4166
+  4699
+  4152
+  3141
+  1210
+  2668
+  1129
+  434
+  299
+  2911
+  2491
+  4624
+  1728
+  3633
+  4465
+  2405
+  3781
+  809
+  1259
+  1775
+  1024
+  1721
+  1693
+  3950
+  3414
+  4323
+  537
+  1129
+  3566
+  61
+  3031
+  3875
+  2990
+  1781
+  2840
+  3042
+  2615
+  4921
+  2250
+  2764
+  3621
+  1585
+  3900
+  1176
+  3092
+  2525
+  665
+  1063
+  3298
+  1306
+  2460
+  3245
+  2257
+  3816
+  4119
+  1337
+  434
+  1245
+  3803
+  1410
+  4561
+  86
+  960
+  3260
+  372

-  1518
-  47
-  1518
-  4460
-  4768
-  3484
-  636
-  4021
-  636
-  2482
-  2482
-  1374
-  267
-  2107
-  2657
-  549
-  2037
-  1947
-  549
-  3976
-  1947
-  2992
-  3654
-  3185
-  3434
-  3484
-  2905
-  3654
-  3636
-  3215
-  4221
-  4121
-  2560
-  1552
-  2992
-  1574
-  684
-  267
-  2107
-  4614
-  1800
-  1333
-  1980
-  105
-  794
-  3945
-  3787
-  3976
-  892
-  4188
-  4453
-  3209
-  4425
-  3945
-  2298
-  2625
-  3688
-  3954
-  912
-  3954
-  4926
-  541
-  1482
-  1460
-  2594
-  1719
-  4725
-  1980
-  3222
-  4221
-  3209
-  3427
-  2002
-  659
-  152
-  2710
-  794
-  873
-  1211
-  4607
-  3336
-  2657
-  2467
-  3302
-  4790
-  1017
-  2036
-  2782
-  3234
-  541
-  4544
-  4713
-  68
-  3553
-  2319
-  2945
-  2002
-  2704
-  356
-  1644
-  4292
-  3775
-  4790
-  3987
-  1781
-  4760
-  3444
-  1807
-  1460
-  729
-  1790
-  2945
-  2625
-  1230
-  4880
-  1405
-  2169
-  311
-  4947
-  2710
-  529
-  1321
-  1744
-  1431
-  4880
-  161
-  979
-  482
-  652
-  334
-  3553
-  873
-  659
-  1017
-  79
-  4482
-  3423
-  202
-  2287
-  3152
-  977
-  39
-  912
-  4227
-  1765
-  4802
-  3984
-  4778
-  2355
-  287
-  263
-  4585
-  4725
-  3733
-  2564
-  1220
-  987
-  2034
-  626
-  4187
-  2615
-  4465
-  1728
-  79
-  1971
-  3014
-  3617
-  3755
-  857
-  3879
-  4425
-  2889
-  4755
-  3365
-  2319
-  1242
-  1619
-  4495
-  4932
-  3686
-  4773
-  4507
-  4010
-  1830
-  4495
-  2057
-  3427
-  1786
-  3421
-  141
-  23
-  3759
-  706
-  523
-  4223
-  4507
-  334
-  1823
-  2381
-  1744
-  3218
-  1576
-  3092
-  597
-  2054
-  933
-  2287
-  4190
-  1800
-  2521
-  4681
-  43
-  1149
-  706
-  2215
-  529
-  1781
-  359
-  43
-  634
-  636
-  2560
-  3344
-  3700
-  2292
-  3365
-  1760
-  3215
-  2139
-  2763
-  4614
-  3028
-  2946
-  4957
-  2292
-  1629
-  1491
-  3922
-  624
-  1431
-  4937
-  1528
-  3177
-  4094
-  4626
-  257
-  2464
-  4624
-  1187
-  1765
-  2168
-  2681
-  1701
-  4243
-  265
-  4531
-  3439
-  2228
-  2215
-  3816
-  3072
-  1824
-  3641
-  1444
-  4482
-  242
-  4766
-  626
-  481
-  3421
-  3344
-  2699
-  4743
-  2835
-  340
-  1210
-  3260
-  645
-  3617
-  287
-  4937
-  4460
-  3593
-  2168
-  352
-  295
-  4766
-  1191
-  1332
-  3921
-  574
-  3466
-  869
-  4778
-  2755
-  214
-  566
-  523
-  1922
-  4802
-  3143
-  4292
-  418
-  2891
-  3987
-  2537
-  510
-  4188
-  3710
-  2115
-  4850
-  4503
-  4474
-  857
-  3775
-  161
-  2525
-  2096
-  3515
-  2763
-  1585
-  732
-  3061
-  1745
-  2670
-  669
-  3686
-  2464
-  1826
-  277
-  3132
-  4389
-  263
-  3049
-  1658
-  3161
-  3209
-  591
-  1187
-  1477
-  78
-  4172
-  2594
-  3733
-  1405
-  3286
-  2833
-  2784
-  3466
-  167
-  2564
-  36
-  1197
-  1707
-  299
-  1629
-  2911
-  1701
-  1357
-  2690
-  896
-  1971
-  3128
-  210
-  4531
-  3414
-  4947
-  1824
-  311
-  4199
-  4483
-  2755
-  295
-  233
-  1126
-  1245
-  763
-  1197
-  3177
-  962
-  690
-  2381
-  2285
-  156
-  3751
-  167
-  781
-  1786
-  4685
-  4425
-  61
-  4187
-  3798
-  4538
-  1888
-  236
-  4713
-  1242
-  4037
-  3210
-  454
-  892
-  4094
-  2578
-  3783
-  912
-  2491
-  398
-  4644
-  2512
-  1962
-  4489
-  843
-  3673
-  282
-  3077
-  3633
-  2645
-  2912
-  126
-  4092
-  4607
-  2896
-  3222
-  4730
-  4067
-  4652
-  159
-  4795
-  4067
-  1823
-  1263
-  3810
-  2512
-  3316
-  4166
-  4699
-  4152
-  3141
-  1210
-  2668
-  1129
-  434
-  299
-  2911
-  2491
-  4624
-  1728
-  3633
-  4465
-  2405
-  3781
-  809
-  1259
-  1775
-  1024
-  1721
-  1693
-  3950
-  3414
-  4323
-  537
-  1129
-  3566
-  61
-  3031
-  3875
-  2990
-  1781
-  2840
-  3042
-  2615
-  4921
-  2250
-  2764
-  3621
-  1585
-  3900
-  1176
-  3092

*/