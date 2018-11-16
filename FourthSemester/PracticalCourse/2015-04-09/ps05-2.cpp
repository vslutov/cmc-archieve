#include <iostream>
#include <string>
#include <vector>
#include <utility>

#include <cinttypes>
#include <cctype>

using namespace std;

typedef pair<string, string> Rule;

static vector<Rule> grammar = vector<Rule>();
static intmax_t type;
static auto left_regular_regular = true, right_regular_regular = true;

static void
init()
{
    while (!cin.eof()) {
        string left, right;
        cin >> left >> right;
        if (!left.empty() && !right.empty()) {
            grammar.push_back(Rule(left, right));
        }
    }
}

static void
calc()
{
    for (auto rule = grammar.begin(); rule != grammar.end(); ++rule) {
        if (rule->first.length() != 1 || !isupper(rule->first[0])) {
            left_regular = false;
            right_regular = false;
            break;
        } else if (rule->second.length() == 2) {
            if (!(isupper(rule->second[0]) && islower(rule->second[1])) {
                left_regular = false;
            } else if (!(islower(rule->second[0]) && isupper(rule->second[1]))) {
                right_regular = false;
            }
        } else if (rule->second.length() == 1) {
            if (isupper(rule->second[0])) {
                left_regular = false;
                right_regular = false;
                break;
            }
        } else {
            left_regular = false;
            right_regular = false;
            break;
        }
    }
                
    if (left_regular || right_regular) {
        type = 3;
    }
}

static void
print()
{
    printf("%jd\n", type);
}

int
main()
{
    init();
    calc();
    print();
}
