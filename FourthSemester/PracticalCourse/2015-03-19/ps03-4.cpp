#include <iostream>

using namespace std;

class A
{
public:
    virtual void a() { cout << 1 << endl; }
    void b() { cout << 2 << endl; }
    virtual void c() { cout << 3 << endl; }
    void d() { cout << 4 << endl; }
    ~A() {}
};

class B : public A
{
public:
    virtual void a() { cout << 5 << endl; }
    virtual void b() { cout << 6 << endl; }
    ~B() {}
};

class C : public A
{
public:
    virtual void b() { cout << 7 << endl; }
    virtual void c() { cout << 8 << endl; }
    virtual void d() { cout << 9 << endl; }
    ~C() {}
};

class D: public B, public C
{
public:
    virtual void e() { cout << 10 << endl; }
    virtual void f() { cout << 11 << endl; }
    ~D() {}
};

class E : public D
{
public:
    virtual void a() { cout << 12 << endl; }
    virtual void d() { cout << 13 << endl; }
    virtual void f() { cout << 14 << endl; }
    ~E() {}
};

int main(void)
{
    A* pa1 = new B;
    B* pb1 = new E;
    C* pc1 = new D;
    B* pb2 = static_cast<B*>(new A);

    pa1->a();
    pa1->b();
    pb1->a();
    pb1->b();
    pb1->c();
    pb1->d();
    pc1->b();
    pc1->c();
    pc1->d();
    pb2->a();
    pb2->d();

    return 0;
}

