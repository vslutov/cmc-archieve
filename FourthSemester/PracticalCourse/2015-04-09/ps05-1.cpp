#include <cstdio>
#include <cctype>

using namespace std;

char next_token = 0;
bool end = false;

void
update()
{
    int token = getchar();
    if (token == -1) {
        end = true;
    } else {
        next_token = token;
    }
}

char
get_token()
{
    if (!end) {
        auto result = next_token;
        update();
        return result;
    } else {
        return 0;
    }
}

void
word()
{
    while (!end && !isspace(next_token)) {
        get_token();
    }
}

void
spaces()
{
    while (!end && isspace(next_token)) {
        get_token();
    }
}

void
alpha()
{
    while (!end && (next_token == '3' || next_token == '4')) {
        get_token();
    }
}

void
beta()
{
    while (!end && (next_token == '1' || next_token == '2')) {
        get_token();
    }
}

void
L()
{
    alpha();
    beta();
    if (end || isspace(next_token)) {
        printf("1\n");
    } else {
        printf("0\n");
    }
    word();
}

int
main()
{
    update();

    while (!end) {
        spaces();
        if (!end) {
            L();
        }
    }
}
