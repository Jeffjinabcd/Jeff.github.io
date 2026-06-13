#include<bits/stdc++.h>
#include<windows.h>
#include<iomanip>
using namespace std;

class Node{
    private: 
        int num;
        int x;
        int h;
        Node** next_node;
    public:
        Node(int = 0, int = 0, int = 0);
        int retrieve_num();
        int retrieve_x();
        int retrieve_h();
        void set_num(int);
        void set_x(int);
        void set_h(int);
        void edit_node(Node*, int);
        Node* Nodes(int);
};

Node::Node(int _num, int _x, int _h){
    num = _num;
    x = _x;
    h = _h;
    next_node = new Node*[_h];
}

int Node::retrieve_num(){
    return num;
}

int Node::retrieve_x(){
    return x;
}

int Node::retrieve_h(){
    return h;
}

void Node::set_num(int y){
    num = y;
}

void Node::set_x(int y){
    x = y;
}

void Node::set_h(int y){
    h = y;
}

void Node::edit_node(Node* z, int x){
    next_node[x] = z;
}

Node* Node::Nodes(int x){
    return next_node[x];
}

class linklist{
    private:
        int length = 0;
        int height = 0;
        Node* listhead;
    public:
        linklist(int);
        bool isempty();
        void print();
        void insert(int, int, int);
        void erase(int, int);
        void search(int, int);
        void print_sim();
        void print_visual();
};

linklist::linklist(int h){
    listhead = new Node(0,0,h);
    height = h;
    for (int i = 0; i < h; i++){
        listhead->edit_node(nullptr, i);
    }
}

bool linklist::isempty(){
    if (length == 0){
        return 0;
    }
    return 1;
}

void linklist::print(){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    cout << "\n" << "{" << "\n";
    for (Node* p = listhead->Nodes(0); p != nullptr; p = p->Nodes(0)){
        
        cout << "[" << "\n(" << setfill(' ') << left << setw(11) << p->retrieve_num() << " ," << setfill(' ') << left << setw(11) << p->retrieve_x() << " ," << setfill(' ') << left << setw(11) << p->retrieve_h() << ")" << "\n";
        for (int j = 0; j < p->retrieve_h(); j++){
            if (p->Nodes(j) == nullptr){
                cout << "(               |            |            |           )" << "\n";
            }
            else{
                cout << "(" << setfill(' ') << left << setw(14) << p->Nodes(j) << " |" << setfill(' ') << left << setw(11) << p->Nodes(j)->retrieve_num() << " |" << setfill(' ') << left << setw(11) << p->Nodes(j)->retrieve_x() << " |" << setfill(' ') << left << setw(11) << p->Nodes(j)->retrieve_h() << ")" << "\n";
            }
        }
        cout << "]" << "\n";
    }
    cout << "}\n";
}

void linklist::print_visual(){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    Node* stuff[length][height];
    int pos = 0;
    for (Node* p = listhead->Nodes(0); p != nullptr; p = p->Nodes(0)){
        for (int i = 0; i < height; ++i){
            if (i < height - p->retrieve_h()){
                stuff[pos][i] = nullptr;
            }
            else{
                stuff[pos][i] = p;
            }
        }
        ++pos;
    }
    cout << "\n";
    for (int i = 0; i < height; ++i){
        cout << "Level " << setfill(' ') << left << setw(11) << height - i << ":\t";
        for (int j = 0; j < length; ++j){
            if (stuff[j][i] != nullptr){
                cout << setfill(' ') << right << setw(11) << stuff[j][i]->retrieve_num() << "|" << setfill(' ') << left << setw(11) << stuff[j][i]->retrieve_x() << " ";
            }
            else{
                cout << "                        ";
            }
        }
        cout << endl;
    }
}

void linklist::print_sim(){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    cout << "\n" << "{";
    for (Node* p = listhead->Nodes(0); p != nullptr; p = p->Nodes(0)){
        cout << "\n(" << p->retrieve_num() << ", " << p->retrieve_x() << ", " << p->retrieve_h() << ")";
    }
    cout << "\n}\n";
}

void linklist::insert(int q, int w, int h){
    Node* p = listhead;
    int e = 1;
    while (rand()%2 == 1 && e < h){
        e++;
    }
    Node* tem[e];
    Node* z = new Node(q,w,e);
    Node* u;
    for (int i = 0; i < e; i++){
        tem[i] = nullptr;
    }
    if (isempty() == 0){
        for (int i = 0; i < e; i++){
            z->edit_node(tem[i], i);
            listhead->edit_node(z, i);
        }
        length++;
    }
    else{
        int i = h - 1;
        while (i >= 0){
            while (p->Nodes(i) != nullptr && p->Nodes(i)->retrieve_num() < q){
                p = p->Nodes(i);
            }
            if (p->Nodes(i) != nullptr && p->Nodes(i)->retrieve_num() == q){
                p->Nodes(i)->set_x(p->Nodes(i)->retrieve_x() + w);
                i = -1;
            }
            else if (i < e){
                tem[i] = p;
            }
            i--;
        }
        if (i == -1){
            for (int j = 0; j < e; j++){
                u = tem[j]->Nodes(j);
                tem[j]->edit_node(z, j);
                z->edit_node(u, j);
            }
            length++;
        }
    }
}

void linklist::erase(int x, int h){
    Node* p = listhead;
    if (isempty() == 0){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
        cout << "\nThe list is empty. Are you stupid? no offense\n";
    }
    else{
        int i = h - 1;
        bool tf = 1;
        while (i >= 0){
            while (p->Nodes(i) != nullptr && p->Nodes(i)->retrieve_num() < x){
                p = p->Nodes(i);
            }
            if (p->Nodes(i) != nullptr && p->Nodes(i)->retrieve_num() == x){
                p->edit_node(p->Nodes(i)->Nodes(i), i);
                tf = 0;
            }
            i--;
        }
        if (tf){
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
            cout << "\nNothing in here, I swear.\n";
        }
        else{
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
            cout << "\nCheck out the new list, I guess.\n";
            length--;
        }
    }
}

void linklist::search(int x, int h){
    Node* p = listhead;
    if (isempty() == 0){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
        cout << "\nThe list is empty. Are you stupid? no offense\n";
    }
    else{
        int i = h - 1;
        while (i >= 0){
            while (p->Nodes(i) != nullptr && p->Nodes(i)->retrieve_num() < x){
                p = p->Nodes(i);
            }
            if (p->Nodes(i) != nullptr && p->Nodes(i)->retrieve_num() == x){
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
                cout << "\nYes, key: " << x << "  value: " << p->Nodes(i)->retrieve_x() << "  height: " << p->Nodes(i)->retrieve_h() << "\n";
                i = -1;
            }
            i--;
        }
        if (i == -1){
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
            cout << "\nNo, gg\n";
        }
    }
}

bool check_int(string x){
    for (int i = 0; i < int(x.length()); i++){
        if (isdigit(x[i]) == false){
            return false;
        }
    }
    return true;
}



int main(){
    srand(time(0));
    int h;
    string g;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
    cout << "Set the height, ty: ";
    cin >> g;
    while(!check_int(g)){
        g = " ";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
        cout << "\nWhat the hell is this?\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
        cout << "\nSet the height, ty: ";
        cin >> g;
    }
    for (int i = 0; i < int(g.length()); i++){
        h = h * 10 + (g[i] - '0');
    }
    linklist a = linklist(h);
    char z;
    cout << "\ninsert, what you want to do ('+' for add(key/value))('-' for erase(key))('@' for search(key))('p' for print())('s' for print_sim()): \n";
    cin >> z;
    int x, y;
    while (z != char('x')){
        if (z != char('x') && z != char('-') && z != char('@') && z != char('p') && z != char('s') && z != char('+') && z != char('f')){
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
            cout << "\nWhat the hell is this?\n";
        }
        else if (z == char('+')){
            cin >> x >> y;
            a.insert(x, y, h);
        }
        else if (z == char('-')){
            cin >> x;
            a.erase(x, h);
        }
        else if (z == char('@')){
            cin >> x;
            a.search(x, h);
        }
        else if (z == char('p')){
            a.print();
        }
        else if (z == char('s')){
            a.print_sim();
        }
        else if (z == char('f')){
            a.print_visual();
        }
        z = char(' ');
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
        cout << "\ninsert, what you want to do ('+' for add(key/value))('-' for erase(key))('@' for search(key))('p' for print())('s' for print_sim()): \n";
        cin >> z;
    }
    a.print();
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}