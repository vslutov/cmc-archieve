#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

static vector<string> term;
static string str, str1, str2;
static ssize_t n, m, k;

static void
save()
{
    for (ssize_t i = 0; i < n; ++i) {
        str[i] = str1[i];
    }
    for (ssize_t i = 0; i < n; ++i) {
        str[n + i] = str1[n - 1 - i];
    }
    for (ssize_t i = 0; i < m; ++i) {
        str[2 * n + i] = str2[i];
    }
    for (ssize_t i = 0; i < m; ++i) {
        str[2 * n + m + i] = str2[m - 1 - i];
    }
    term.push_back(str);
}

static void
rec2(ssize_t i)
{
    if (i == m) {
        save();
    } else {
        str2[i] = '1';
        rec2(i + 1);
        str2[i] = '2';
        rec2(i + 1);
    }
}

static void
rec1(ssize_t i)
{
    if (i == n) {
        rec2(0);
    } else {
        str1[i] = '3';
        rec1(i + 1);
        str1[i] = '4';
        rec1(i + 1);
    }
}

int
main()
{
    cin >> k;

    str = string(k, '0');
    for (n = 0; n <= k / 2; ++n) {
        m = k / 2 - n;
        str1 = string(n, '0');
        str2 = string(m, '0');
        rec1(0);
    }

    sort(term.begin(), term.end());

    for (auto pstr = term.begin(); pstr != term.end(); ++pstr) {
        cout << *pstr << endl;
    }
}
