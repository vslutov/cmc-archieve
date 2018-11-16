#include <iostream>
#include <string>

using namespace std;

auto
count(string::const_iterator &iter,
      const string::const_iterator &end,
      char ch)
{
    ssize_t result = 0;
    while (iter != end && *iter == ch) {
        ++ result;
        ++ iter;
    }

    return result;
}

auto
is_str(const string &test_str)
{
    auto iter = test_str.cbegin();
    count(iter, test_str.cend(), 'a');
    auto b = count(iter, test_str.cend(), 'b');

    return iter == test_str.cend() && b > 0;
}

int
main()
{
    cout << 0 << endl << 3 << endl;

    while (!cin.eof()) {
        string test_str;
        cin >> test_str;
        if (!cin.eof()) {
            cout << is_str(test_str) << endl;
        }
    }
}
