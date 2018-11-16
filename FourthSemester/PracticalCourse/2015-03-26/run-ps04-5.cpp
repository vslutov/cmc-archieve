#include "ps04-5.cpp"
#include <iostream>

using namespace std;

int
main()
{
    cout << Fib<0>::VAL << endl;
    cout << Fib<1>::VAL << endl;
    cout << Fib<2>::VAL << endl;
    cout << Fib<3>::VAL << endl;
    cout << Fib<4>::VAL << endl;
    cout << Fib<5>::VAL << endl;
}
