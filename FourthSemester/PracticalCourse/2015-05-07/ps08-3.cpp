/*
 * %O% - Operation [+, -, *, /, %, (, )]
 * %D% - Digit [0 - 9]
 *
 * S -> %O%S | %D%A | .
 * A -> %O%S | %D%A | .
 *
 * */

#include <cctype>
#include <iostream>

using namespace std;

int
main()
{
    char next;
    cin >> next;

    char q = 'S';

    while (!cin.eof()) {
        if (q == 'S') {
            if (isdigit(next)) {
                q = 'A';
                cout << next;
            } else if (next == '.') {
                break;
            } else {
                cout << next << endl;
            }
        } else {
            if (isdigit(next)) {
                cout << next;
            } else if (next == '.') {
                cout << endl;
                break;
            } else {
                q = 'S';
                cout << endl << next << endl;
            }
        }

        cin >> next;
    }
}
