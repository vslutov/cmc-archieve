#include <iostream>
#include <vector>
#include <string>

using namespace std;

static void
rec (ssize_t k, string &result, ssize_t i, bool has_two)
{
    if (i == k) {
        cout << result << endl;
        return;
    }

    result[i] = '1';
    rec(k, result, i + 1, true);
    result[i] = '2';
    rec(k, result, i + 1, true);

    if (!has_two) {
        result[i] = '3';
        rec(k, result, i + 1, false);
        result[i] = '4';
        rec(k, result, i + 1, false);
    }
}

int
main()
{
    ssize_t k;
    cin >> k;

    string result;

    for (ssize_t i = 0; i < k; ++ i) {
        result += "1";
    }

    rec(k, result, 0, false);
}
