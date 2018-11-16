#include "ps04-2.cpp"
#include <iostream>
#include <vector>

using namespace std;

int
main()
{
    size_t size = 6;
    vector<int> a = vector<int>(size);
    for(size_t i = 0; i < size; ++i) {
        a[i] = i;
    }
    cout << process(a) << endl;
}
