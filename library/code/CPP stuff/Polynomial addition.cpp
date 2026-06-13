#include<bits/stdc++.h>
using namespace std;


class Node {
    private: 
        float num;
        float x;
        Node* next_node;
    public:
        Node(float = 0, float = 0, Node* = nullptr);
        float retrieve_num();
        float retrieve_x();
        void set_num(float);
        void set_x(float);
        void set_next_node(Node*);
        Node* next();
};

Node::Node(float _num, float _x, Node* _next_node){
    num = _num;
    x = _x;
    next_node = _next_node;
}

float Node::retrieve_num(){
    return num;
}

float Node::retrieve_x(){
    return x;
}

void Node::set_num(float x){
    num = x;
}

void Node::set_x(float y){
    x = y;
}

void Node::set_next_node(Node* z){
    next_node = z;
}

Node* Node::next(){
    return next_node;
}

class linklist{
    private:
        Node* listhead;
        int length;
    public:
        linklist();
        bool isempty();
        float getlength();
        float front();
        Node* head();
        void print();

        void pushfront(float, float);
        void popfront();
        int erase(int);

        void sort(float, float);
        linklist operator+(linklist &v);
};

linklist::linklist(){
    listhead = nullptr;
    length = 0;
}

bool linklist::isempty(){
    if (listhead == nullptr){
        return 0;
    }
    return 1;
}

float linklist::getlength(){
    return length;
}

float linklist::front(){
    if (isempty()){
        return listhead->retrieve_num();
    }
    return -1;
}

Node* linklist::head(){
    if (isempty()){
        return listhead;
    }
    return nullptr;
}

void linklist::print(){
    cout << "[";
    for (Node* p = listhead; p!= nullptr; p = p->next()){
        cout << p->retrieve_num() << "^" << p->retrieve_x();
        if (p->next() != nullptr){
            cout << ", ";
        }
    }
    cout << "]";
}

void linklist::pushfront(float num, float x){
    listhead = new Node(num, x, listhead);
    length++;
}

void linklist::popfront(){
    if (!isempty()){
        Node* p = listhead;
        p = p->next();
        listhead->set_next_node(p);
    }
}

void linklist::erase(){
    listhead = nullptr;
}

void linklist::sort(float q, float w){
    Node* p = listhead;
    Node* x = listhead;
    if (isempty()){
        if (p->retrieve_x() > w){
            listhead = new Node(q, w, listhead);
        }
        else if (p->retrieve_x() == w){
            p->set_num(p->retrieve_num() + q);
        }
        else{
            while (p != nullptr && p->retrieve_x() < w ){
                x = p;
                p = p->next();
            }
            
            if (p != nullptr){
                if (p->retrieve_x() == w){
                    p->set_num(p->retrieve_num() + q);
                }
                else{
                    Node* z = new Node;
                    z->set_num(q);
                    z->set_x(w);
                    z->set_next_node(p);
                    x->set_next_node(z);
                }
            }
            else{
                Node* z = new Node;
                z->set_num(q);
                z->set_x(w);
                z->set_next_node(nullptr);
                x->set_next_node(z);
            }
        }
    }
    else{
        pushfront(q, w);
    }
}

linklist linklist::operator+(linklist &v){
    linklist r;
    Node* vv = v.listhead;
    Node* pp = listhead;
    while (vv != nullptr && pp != nullptr){
    //for (Node* p = listhead; p!= nullptr; p = p->next()){
        if (pp->retrieve_x() == vv->retrieve_x()){
            r.pushfront(pp->retrieve_num()+vv->retrieve_num(), pp->retrieve_x());
            pp = pp->next();
            vv = vv->next();
        }
        else if (pp->retrieve_x() < vv->retrieve_x()){
            r.pushfront(pp->retrieve_num(), pp->retrieve_x());
            pp = pp->next();
        }
        else{
            r.pushfront(vv->retrieve_num(), vv->retrieve_x());
            vv = vv->next();
        }
    }
    if (vv == nullptr && pp != nullptr){
        for (Node* ppp = pp; ppp != nullptr; ppp = ppp->next()){
            r.pushfront(ppp->retrieve_num(), ppp->retrieve_x());
            return r;
        }
    }
    else if (pp == nullptr && vv != nullptr){
        for (Node* vvv = vv; vvv != nullptr; vvv = vvv->next()){
            r.pushfront(vvv->retrieve_num(), vvv->retrieve_x());
            return r;
        }
    }
    else{
        return r;
    }
}
/*
class vector2f{
    private:
        int x, y;
    public:
        vector2f(int, int);
        int get_x();
        int get_y();
        void set_x(int);
        void set_y(int);
        vector2f operator+(vector2f &v);
};

vector2f::vector2f(int _x, int _y){
    x = _x;
    y = _y;
}

int vector2f::get_x(){
    return x;
}
int vector2f::get_y(){
    return y;
}
void vector2f::set_x(int i){
    x = i;
}
void vector2f::set_y(int i){
    y = i;
}

vector2f vector2f::operator+(vector2f &v){
    vector2f r(0, 0);
    r.x = x + v.x;
    r.y = y + v.y;
    return r;
}
*/
int main(){
    freopen("cowsignal.in", "r", stdin);
	freopen("cowsignal.out", "w", stdout);
    linklist a, b;
    float q, w;
    int z;
    cin >> z;
    cin >> q >> w;
    while (q != -2){
        a.sort(w, q);
        a.print();
        cout << endl;
        cin >> q;
        if (q == -2){}
        if (q != -2){
            cin >> w;
        }
    }
    cin >> q >> w;
    while (q != -3){
        b.sort(w, q);
        b.print();
        cout << endl;
        cin >> q;
        if (q == -3){}
        if (q != -3){
            cin >> w;
        }
    }
    a.print();
    b.print();
    linklist r = a+b;
    cout << "\n\n";
    r.print();
}