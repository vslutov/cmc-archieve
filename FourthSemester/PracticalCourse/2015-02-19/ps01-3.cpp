#include <iostream>

using namespace std;

void
f (double, char c = '0')
{
    cout << "2" << endl;
}

void
f (int, const char *)
{
    cout << "2" << endl;
}
