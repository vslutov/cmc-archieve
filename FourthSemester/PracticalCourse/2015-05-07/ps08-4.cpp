/*
 * S -> T + S | T - S | T
 * T -> F * T | F / T | F % T | F
 * F -> %Number% | ( S )
 */

#include <string>
#include <cinttypes>
#include <iostream>
#include <cstdlib>

using namespace std;

static string token;
bool eof = false;

void
eat_token()
{
    if (eof) {
        cout << "ERROR unexpected eof" << endl;
        exit(0);
    }
    cin >> token;
    eof = cin.eof();
}

intmax_t
parse_S();

intmax_t
parse_F()
{
    intmax_t value;

    if (token == "(") {
        eat_token();
        value = parse_S();
        if (token == ")") {
            eat_token();
        } else {
            cout << "ERROR unexpected '" << token << "'" << endl;
            exit(0);
        }
    } else {
        value = stol(token);
        eat_token();
    }

    return value;
}

intmax_t
parse_T()
{
    intmax_t value = parse_F();

    while (token == "*" || token == "/" || token == "%") {
        if (token == "*") {
            eat_token();
            value *= parse_F();
        } else if (token == "/") {
            eat_token();
            intmax_t temp = parse_F();
            if (temp != 0) {
                value /= parse_F();
            } else {
                cout << "ERROR division by zero" << endl;
                exit(0);
            }
        } else {
            eat_token();
            intmax_t temp = parse_F();
            if (temp != 0) {
                value %= parse_F();
            } else {
                cout << "ERROR division by zero" << endl;
                exit(0);
            }
        }
    }

    return value;
}

intmax_t
parse_S()
{
    intmax_t value = parse_T();

    while (token == "+" || token == "-") {
        if (token == "+") {
            eat_token();
            value += parse_T();
        } else if (token == "-") {
            eat_token();
            value -= parse_T();
        }
    }

    return value;
}

int
main()
{
    ios::sync_with_stdio(false);

    cin >> token;

    intmax_t value = parse_S();

    if (eof) {
        cout << value << endl;
    } else {
        cout << "ERROR we need to stop it" << endl;
    }
}
