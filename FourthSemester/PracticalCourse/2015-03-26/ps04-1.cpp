#include <vector>
#include <iostream>

using namespace std;

void
process(vector<int> &v)
{
    for (auto x = v.begin(); x < v.end(); ++x) {
        v.erase(x);
    }

    for (auto x = v.rbegin(); x != v.rend(); ++x) {
        cout << *x << ' ';
    }
    cout << endl;
}
