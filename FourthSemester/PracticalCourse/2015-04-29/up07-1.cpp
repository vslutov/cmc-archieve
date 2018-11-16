#include <iostream>
#include <string>

using namespace std;

auto
count(string::iterator &iter,
      const string::iterator &end,
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
is_right(string &sample)
{
    auto iter = sample.begin();
    auto a = count(iter, sample.end(), 'a');
    auto zero = count(iter, sample.end(), '0');
    auto b = count(iter, sample.end(), 'b');
    auto one = count(iter, sample.end(), '1');

    return iter == sample.end() &&
           a == b &&
           a > 0 &&
           zero == one;
}

int
main()
{
    ios::sync_with_stdio(false);
    string sample;

    while (!cin.eof()) {
        cin >> sample;
        if (!cin.eof()) {
            cout << is_right(sample) << endl;
        }
    }
}
