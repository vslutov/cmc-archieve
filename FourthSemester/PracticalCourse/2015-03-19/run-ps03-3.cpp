#include "ps03-3.cpp"

int main(void)
{
    C c1(7), c2 = 5, c3(c1 + c2);
    cout << c1.get() << ' ' << c2.get() << ' ' << c3.get() << endl;
    return 0;
}
