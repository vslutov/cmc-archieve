#include <iostream>
using namespace std;

class A;
void g(A& x, int n);

class A
{
public:
    A()
    {
        try {
            g(*this, 0);
            cout << 'Z' << endl;
        } catch (A &) {
            cout << 'X' << endl;
        } catch (int n) {
            g(*this, -n);
            cout << 'T' << endl;
        }
    }
    A(A& a)
    {
        cout << 'B' << endl;
    }
    ~A()
    {
        cout << 'A' << endl;
    }
};

void g(A& x, int n)
{
    try {
        if (n <= 0) throw x;
        if (n >= 0) throw n - 1;
        cout << 'N' << endl;
    } catch (int) {
        cout << 'P' << endl;
        throw;
    } catch (A&) {
        cout << 'O' << endl;
        throw;
    }
}

int main()
{
    try {
        A a;
    } catch(...) {
        cout << 'V' << endl;
    }
    cout << 'C' << endl;
}

