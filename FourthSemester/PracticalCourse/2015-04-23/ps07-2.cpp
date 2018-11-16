#include <string>
#include <iostream>

using namespace std;

static bool
is_str(const string &s)
{
    if (s.length() < 2) {
        return false;
    } else {
        char q = 'D';

        string suff = s.substr(s.length() - 2);
        if ((suff[0] != '0' && suff[0] != '1') ||
            (suff[1] != '0' && suff[1] != '1')) {
            return false;
        }

        string pre = s.substr(0, s.length() - 2);
        for (auto p = pre.begin(); p != pre.end(); ++p) {
            if (*p == '0') {
                q = 'A';
            } else if (*p == '1') {
                q = 'X';
            } else {
                return false;
            }
        }

        return q == 'X';
    }
}

int
main()
{
    while (!cin.eof()) {
        string s;
        cin >> s;
        if (!cin.eof()) {
            cout << int(is_str(s)) << endl;
        }
    }
}
