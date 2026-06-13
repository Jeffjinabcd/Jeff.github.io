#include<bits/stdc++.h>
using namespace std;

class Vector2f{
    public:
        enum Mode{RECT, POL};
    private:
        double x;
        double y;
        double mag;
        double ang;
        Mode mode;
        void setMag();
        void setAng();
        void setX();
        void setY();
    public:
        Vector2f();
        Vector2f(double n1, double n2, Mode form = RECT);
        void reset(double n1, double n2, Mode form = RECT);
        double getX();
        double getY();
        double getMag();
        double getAng();
        void polMode();
        void rectMode();
        void easiest_print();

        Vector2f operator+(Vector2f &b);
        Vector2f operator-(Vector2f &b);
        Vector2f operator-();
        Vector2f operator*(double);
        Vector2f longer(Vector2f &v);
        friend Vector2f operator*(double, Vector2f &a);
        friend std::ostream& operator<< (ostream &os, Vector2f &a);
};

Vector2f::Vector2f(double n1, double n2, Mode form){
    x = n1;
    y = n2;
    if (abs(n1) == 0){
        x = 0;
    }
    if (abs(n2) == 0){
        y = 0;
    }
    mode = form;
    setMag();
    setAng();
}

void Vector2f::reset(double n1, double n2, Mode form){
    x = n1;
    y = n2;
    if (abs(n1) == 0){
        x = 0;
    }
    if (abs(n2) == 0){
        y = 0;
    }
    mode = form;
    setMag();
    setAng();
}

void Vector2f::setMag(){
    mag = sqrt(x*x + y*y);
}

void Vector2f::setAng(){
    if (x > 0 && y > 0){
        ang = atan(abs(y/x))/3.1415926*180;
    }
    else if (x < 0 && y > 0){
        ang = 180-atan(abs(y/x))/3.1415926*180;
    }
    else if (x < 0 && y < 0){
        ang = 180+atan(abs(y/x))/3.1415926*180;
    }
    else if (x > 0 && y < 0){
        ang = 360-atan(abs(y/x))/3.1415926*180;
    }
    else if (x == 0 && y == 0){
        ang = 0;
    }
    else if (x == 0){
        if (y > 0){
            ang = 90;
        }
        else{
            ang = 270;
        }
    }
    else if (y == 0){
        if (x > 0){
            ang = 0;
        }
        else{
            ang = 180;
        }
    }
}

void Vector2f::setX(){
    x = cos(ang*3.1415926/180)*mag;
}

void Vector2f::setY(){
    y = sin(ang*3.1415926/180)*mag;
}

double Vector2f::getMag(){
    return mag;
}

double Vector2f::getAng(){
    return ang;
}

double Vector2f::getX(){
    return x;
}

double Vector2f::getY(){
    return y;
}

void Vector2f::polMode(){
    mode = POL;
}

void Vector2f::rectMode(){
    mode = RECT;
}

void Vector2f::easiest_print(){
    cout << getX() << "\t" << getY() << "\t" << getMag() << "\t" << getAng() << "\n";
}

Vector2f Vector2f::operator+(Vector2f &b){
    Vector2f tem = Vector2f(getX() + b.getX(), getY() + b.getY());
    return tem;
}

Vector2f Vector2f::operator-(Vector2f &b){
    Vector2f tem = Vector2f(getX() - b.getX(), getY() - b.getY());
    return tem;
}

Vector2f Vector2f::operator-(){
    Vector2f tem = Vector2f(-getX(), -getY());
    return tem;
}

Vector2f Vector2f::operator*(double p){
    Vector2f tem = Vector2f(getX()*p, getY()*p);
    return tem;
}

Vector2f Vector2f::longer(Vector2f &v){
    if (v.getMag() > getMag()){
        return v;
    }
    else{
        return *this;
    }
}

Vector2f operator*(double p, Vector2f &a){
    Vector2f tem = Vector2f(a.getX()*p, a.getY()*p);
    return tem;
}

ostream& operator<< (ostream& os, Vector2f& a){
    os << "[" << "(Mag= " << a.getMag() << "\t, Ang= " << a.getAng() << "\t), (X= " << a.getX() << "\t, Y= " << a.getY() << "\t)]\n";
    return os;
}

/*
为什么减号是前面减后面但是负号要放在前面。(1, 2) - (3, 4) = (-2, -2)        -(1, 2) = (-1, -2) operator一个在后一个在前。
longer前面的&跟其他的是一样的吗？
如果把friend的两个引用换位置把原来的*注释掉，为什么a*2用不了了？
//Vector2f Vector2f::operator*(double p){
//    Vector2f tem = Vector2f(getX()*p, getY()*p);
//    return tem;
//}
Vector2f operator*(Vector2f &a, double p){
    Vector2f tem = Vector2f(a.getX()*p, a.getY()*p);
    return tem;
}
os在<<里面就是用原来的cout << 来打印的吗？ostream后面&是干什么？
Vector2f operator+(Vector2f &b);
Vector2f operator-(Vector2f &b);
Vector2f operator-();
Vector2f operator*(double);
Vector2f& longer(Vector2f &v);
friend Vector2f operator*(double, Vector2f &a);
friend std::ostream& operator<< (ostream &os, Vector2f &a);
*/

int main(){
    int y, u, i, o;
    cin >> y >> u >> i >> o;
    Vector2f a = Vector2f(y, u);
    Vector2f b = Vector2f(i, o);
    Vector2f d = (a+b)*2;
    cout << d;
    
}