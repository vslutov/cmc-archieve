#include "ps04-3.cpp"
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
    myreverse(a.begin(), a.end());
    for (auto x = a.begin(); x < a.end(); ++x) {
        cout << *x << ' ';
    }
    cout << endl;
}
