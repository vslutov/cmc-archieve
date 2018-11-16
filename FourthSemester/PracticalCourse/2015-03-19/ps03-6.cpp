#include <iostream>
using namespace std;
int k = 2;
class A {
public:
    virtual int g(int x) {
        h();
        cout << "A::f," << x << endl;
        return 1;
    }
    void h() {
        t();
        cout << "A::g" << endl;
    }
    virtual void t() {
        cout << "A::h," << k << endl;
    }
};
class B: public A {
public:
    int g(double y) {
        h();
        cout << "B::f," << y << endl;
        return 3;
    }
    virtual void h() {
        t(1);
        cout << "B::g" << endl;
    }
    virtual void t(int k) {
        cout << "B::h," << k << endl;
    }
};
int
main()
{
    A a; B b; A *p = &b;
    p->g(1.5);
    p->h();
    p->t();
}
