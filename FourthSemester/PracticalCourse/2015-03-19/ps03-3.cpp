#include <iostream>
using namespace std;

class C {
    int value;
public:
    C(int x) : value(2 * x) {};
    int get() const
    {
        return value;
    }
};

C operator+(const C &x, const C &y)
{
    return C(x.get() + y.get());
}
