#include<bits/stdc++.h>
#include<windows.h>
#include<iomanip>
using namespace std;

void text_color(string x){
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
    if (x == "Blue"){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
    }
    else if (x == "Green"){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
    }
    else if (x == "Cyan"){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
    }
    else if (x == "Red"){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
    }
    else if (x == "Purple"){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5);
    }
    else if (x == "Dark Yellow"){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
    }
    else if (x == "White"){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    }
    else if (x == "Grey"){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
    }
    else if (x == "Bright Blue"){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
    }
    else if (x == "Bright Green"){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
    }
    else if (x == "Bright Cyan"){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    }
    else if (x == "Bright Red"){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
    }
    else if (x == "Pink"){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
    }
    else if (x == "Yellow"){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    }
    else if (x == "Bright White"){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    }
}

class Node{
    private:
        float Num;
        Node* Left;
        Node* Right;
        int Child;
    public:
        Node(float _Num = 0, Node* _Left = nullptr, Node* _Right = nullptr);
        float GetNum();
        Node* GetLeft();
        Node* GetRight();
        int GetChild();
        void SetNum(float);
        void SetLeft(Node*);
        void SetRight(Node*);
        void Print();
};

Node::Node(float _Num, Node* _Left, Node* _Right){
    Num = _Num;
    Left = _Left;
    Right = _Right;
    Child = 0;
    if (Left != nullptr){
        ++Child;
    }
    if (Right != nullptr){
        ++Child;
    }
}

float Node::GetNum(){
    return Num;
}

Node* Node::GetLeft(){
    return Left;
}

Node* Node::GetRight(){
    return Right;
}

int Node::GetChild(){
    return Child;
}

void Node::SetNum(float _Num){
    Num = _Num;
}

void Node::SetLeft(Node* _Left){
    Left = _Left;
    Child = 0;
    if (Left != nullptr){
        ++Child;
    }
    if (Right != nullptr){
        ++Child;
    }
}

void Node::SetRight(Node* _Right){
    Right = _Right;
    Child = 0;
    if (Left != nullptr){
        ++Child;
    }
    if (Right != nullptr){
        ++Child;
    }
}

void Node::Print(){
    cout << Num << endl;
    cout << Left << "\t";
    if (Left != nullptr){
        cout << Left->GetNum() << endl;
    }
    else{
        cout << endl;
    }
    cout << Right << "\t";
    if (Right != nullptr){
        cout << Right->GetNum() << endl;
    }
    else{
        cout << endl;
    }
    cout << endl;
}

class Binary{
    private:
        Node* Binaryhead;
        int length;
    public:
        Binary();
        int Getlength();
        void tree_recur(Node*);
        void min_max_recur(Node*);
        Node* mini();
        Node* sub_mini(Node*);
        Node* somehow_i_want_this_shit_last_to_sub_mini_if_possible(Node*);
        Node* max();
        bool exist(float);
        Node* find(float);
        Node* exist_but_node(float);
        void insert(float);
        void erase(float);
        void tree_print_kind_of();
        void print_mini_to_max();
};

Binary::Binary(){
    Binaryhead = new Node(0);
    length = 0;
}

int Binary::Getlength(){
    return length;
}

void Binary::tree_recur(Node* x){
    x->Print();
    if (x->GetLeft() != nullptr){
        tree_recur(x->GetLeft());
    }
    if (x->GetRight() != nullptr){
        tree_recur(x->GetRight());
    }
}

void Binary::min_max_recur(Node* x){
    if (x->GetLeft() != nullptr){
        min_max_recur(x->GetLeft());
    }
    x->Print();
    if (x->GetRight() != nullptr){
        min_max_recur(x->GetRight());
    }
}

Node* Binary::mini(){
    Node* temp = Binaryhead;
    if (length == 0){
        cout << "\nEmpty\n\n";
        return nullptr;
    }
    else{
        while (temp->GetLeft() != nullptr){
            temp = temp->GetLeft();
        }
        return temp;
    }
}

Node* Binary::sub_mini(Node* x){
    Node* temp = x;
    if (length == 0){
        cout << "\nEmpty\n\n";
        return nullptr;
    }
    else{
        while (temp->GetLeft() != nullptr){
            temp = temp->GetLeft();
        }
        return temp;
    }
}

Node* Binary::somehow_i_want_this_shit_last_to_sub_mini_if_possible(Node* x){
    Node* temp = x;
    if (length == 0){
        cout << "\nEmpty\n\n";
        return nullptr;
    }
    else{
        if (temp->GetLeft() == nullptr){
            return temp;
        }
        while (temp->GetLeft()->GetLeft() != nullptr){
            temp = temp->GetLeft();
        }
        return temp;
    }
}

Node* Binary::max(){
    Node* temp = Binaryhead;
    if (length == 0){
        cout << "\nEmpty\n\n";
        return nullptr;
    }
    else{
        while (temp->GetRight() != nullptr){
            temp = temp->GetRight();
        }
        return temp;
    }
}

bool Binary::exist(float x){
    Node* temp = Binaryhead;
    if (length == 0){
        return 0;
    }
    else{
        while (1){
            if (temp->GetNum() > x){
                if (temp->GetLeft() != nullptr){
                    temp = temp->GetLeft();
                }
                else{
                    return 0;
                }
            }
            else if (temp->GetNum() < x){
                if (temp->GetRight() != nullptr){
                    temp = temp->GetRight();
                }
                else{
                    return 0;
                }
            }
            else{
                return 1;
            }
        }
    }
}

Node* Binary::find(float x){
    Node* temp = Binaryhead;
    if (length == 0){
        cout << "\nEmpty\n\n";
        return 0;
    }
    else{
        while (1){
            if (temp->GetNum() > x){
                temp = temp->GetLeft();
            }
            else if (temp->GetNum() < x){
                temp = temp->GetRight();
            }
            else{
                return temp;
            }
        }
    }
}

Node* Binary::exist_but_node(float x){
    Node* temp = Binaryhead;
    if (length == 0){
        return nullptr;
    }
    else{
        while (1){
            if (temp->GetNum() > x){
                if (temp->GetLeft() != nullptr){
                    temp = temp->GetLeft();
                }
                else{
                    return nullptr;
                }
            }
            else if (temp->GetNum() < x){
                if (temp->GetRight() != nullptr){
                    temp = temp->GetRight();
                }
                else{
                    return nullptr;
                }
            }
            else{
                return temp;
            }
        }
    }
}

void Binary::insert(float x){
    if (length == 0){
        Binaryhead->SetNum(x);
    }
    else{
        Node* last = Binaryhead;
        bool switch1 = 1;
        while (switch1){
            if (last->GetNum() > x && last->GetLeft() != nullptr){
                last = last->GetLeft();
            }
            else if (last->GetNum() < x && last->GetRight() != nullptr){
                last = last->GetRight();
            }
            else{
                switch1 = 0;
            }
        }
        if (last->GetNum() == x){
            cout << "\nSame number\n\n";
        }
        else if (last->GetNum() > x){
            Node* z = new Node(x, nullptr, nullptr);
            last->SetLeft(z);
        }
        else{
            Node* z = new Node(x, nullptr, nullptr);
            last->SetRight(z);
        }
    }
    ++length;
}

void Binary::erase(float x){
    if (length == 0){
        cout << "\nEmpty\n\n";
    }
    else{
        bool switch1 = 1;
        Node* temp = Binaryhead;
        Node* last = nullptr;
        while (switch1 && temp->GetNum() != x){
            if (temp->GetNum() > x){
                if (temp->GetLeft() != nullptr){
                    last = temp;
                    temp = temp->GetLeft();
                }
                else {
                    switch1 = 0;
                }
            }
            else if (temp->GetNum() < x){
                if (temp->GetRight() != nullptr){
                    last = temp;
                    temp = temp->GetRight();
                }
                else {
                    switch1 = 0;
                }
            }
        }
        if (!switch1){
            cout << "\nDNE\n\n";
        }
        else {
            if (length == 1){
                Binaryhead->SetNum(0);
            }
            else if (temp->GetChild() < 2){
                    if (last == nullptr){
                        if (temp->GetRight() == nullptr){
                            temp->SetNum(temp->GetLeft()->GetNum());
                            temp->SetRight(temp->GetLeft()->GetRight());
                            temp->SetLeft(temp->GetLeft()->GetLeft());
                        }
                        else{
                            temp->SetNum(temp->GetRight()->GetNum());
                            temp->SetLeft(temp->GetRight()->GetLeft());
                            temp->SetRight(temp->GetRight()->GetRight());
                        }
                    }
                    else{
                        if (last->GetNum() < temp->GetNum()){
                            if (temp->GetRight() == nullptr){
                                last->SetRight(temp->GetLeft());
                            }
                            else{
                                last->SetRight(temp->GetRight());
                            }
                        }
                        else{
                            if (temp->GetRight() == nullptr){
                                last->SetLeft(temp->GetLeft());
                            }
                            else{
                                last->SetLeft(temp->GetRight());
                            }
                        }
                    }
                }
            else{
                Node* u = somehow_i_want_this_shit_last_to_sub_mini_if_possible(temp->GetRight());
                if (u == temp->GetRight()){
                    temp->SetNum(u->GetNum());
                    temp->SetRight(u->GetRight());
                }
                else{
                    temp->SetNum(u->GetLeft()->GetNum());
                    u->SetLeft(u->GetLeft()->GetRight());
                }
            }
            --length;
        }
    }
}

void Binary::tree_print_kind_of(){
    if (length == 0){
        cout << "\nEmpty\n\n";
    }
    else{
        cout << "\n";
        tree_recur(Binaryhead);
    }
}

void Binary::print_mini_to_max(){
    if (length == 0){
        cout << "\nEmpty\n\n";
    }
    else{
        cout << "\n";
        min_max_recur(Binaryhead);
    }
}

bool check_float(string x){
    for (int i = 0; i < int(x.length()); i++){
        if (isdigit(x[i]) == false && x[i] != char('.')){
            return false;
        }
    }
    return true;
}

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
            }
            else{
                h = h * 10 + (x[i] - '0');
            }
        }
    }
    return h;
}

int main(){
    Binary a;
    char z;
    string y;
    text_color("Grey");
    cout << "\nInsert(+)|Erase(-)|Find(@)|Tree_Print(p)|Min_Max_Print(o)|Quit(x)\n";
    cin >> z;
    float x;
    Node* j;
    while (z != char('x')){
        text_color("Bright Blue");
        if (z != char('x') && z != char('+') && z != char('-') && z != char('@') && z != char('p') && z != char('o')){
            cout << "\nWhat the hell is this?\n";
        }
        else if (z == char('+')){
            cin >> y;
            if (check_float(y)){
                x = to_float(y);
                a.insert(x);
            }
            else{
                cout << "Is that a number?" << endl;
            }
        }
        else if (z == char('-')){
            cin >> y;
            if (check_float(y)){
                x = to_float(y);
                a.erase(x);
            }
            else{
                cout << "Is that a number?" << endl;
            }
        }
        else if (z == char('@')){
            cin >> y;
            if (check_float(y)){
                x = to_float(y);
                j = a.exist_but_node(x);
                if (a.Getlength() == 0){
                    cout << "\nEmpty\n\n";
                }
                else if (j != nullptr){
                    cout << endl;
                    j->Print();
                }
                else{
                    cout << "\nDNE\n\n";
                }
            }
            else{
                cout << "Is that a number?" << endl;
            }
        }
        else if (z == char('p')){
            a.print_mini_to_max();
        }
        else if (z == char('o')){
            a.tree_print_kind_of();
        }
        text_color("Grey");
        cout << "\nInsert(+)|Erase(-)|Find(@)|Print(p)|Quit(x)\n";
        cin >> z;
    }
}

/*
+31
+45
+36
+14
+52
+42
+6
+21
+73
+47
+26
+37
+33
+8
*/