#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>

using namespace std;

typedef map<char, string> rule;

static map<string, rule> DKA;
static set<string> TERM;
static string START, TEST;

static void
scan()
{
    bool end = false;
    while (!end) {
        string a;
        cin >> a;
        if (a == "END") {
            end = true;
        } else {
            char b;
            string c;
            cin >> b >> c;
            DKA[a][b] = c;
        }
    }
    end = false;
    while (!end) {
        string a;
        cin >> a;
        if (a == "END") {
            end = true;
        } else {
            TERM.insert(a);
        }
    }

    cin >> START >> TEST;
}

static void
calc()
{
    auto q = START;
    bool ok = true;
    ssize_t count = 0;
    for (auto p = TEST.begin(); p != TEST.end(); ++p) {
        if (DKA[q].count(*p) == 0) {
            ok = false;
            break;
        } else {
            count += 1;
            q = DKA[q][*p];
        }
    }

    if (ok) {
        ok = TERM.count(q) > 0;
    }

    cout << int(ok) << endl << count << endl << q << endl;
}

int
main()
{
    scan();
    calc();
}
