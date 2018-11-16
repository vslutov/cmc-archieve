#include <iostream>

using namespace std;

int
main()
{
    ios::sync_with_stdio(false);
    ssize_t k;
    cin >> k;

    if (k % 2 == 0) {
        for (ssize_t m = 1; m < k / 2; ++ m) {
            ssize_t n = k / 2 - m;
            for (ssize_t i = 0; i < n; ++i) {
                cout << 'a';
            }
            for (ssize_t i = 0; i < m; ++i) {
                cout << 'b';
            }
            for (ssize_t i = 0; i < m; ++i) {
                cout << 'c';
            }
            for (ssize_t i = 0; i < n; ++i) {
                cout << 'd';
            }
            cout << endl;
        }
    }
}
