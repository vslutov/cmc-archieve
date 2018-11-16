#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <utility>

#include <cinttypes>
#include <cctype>

using namespace std;

typedef pair<string, string> Rule;
typedef vector<Rule> Grammar;

static Grammar grammar = Grammar();

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
    set<char> reachable;
    auto last_step = reachable;
    reachable.insert('S');

    while (last_step.size() != reachable.size()) {
        last_step = reachable;

        for (auto rule = grammar.begin(); rule != grammar.end(); ++rule) {
            if (last_step.count(rule->first[0])) {
                for (auto NT = rule->second.begin(); NT != rule->second.end(); ++NT) {
                    if (isupper(*NT) && last_step.count(*NT) == 0) {
                        reachable.insert(*NT);
                    }
                }
            }
        }
    }

    Grammar clear_grammar;

    for (auto rule = grammar.begin(); rule != grammar.end(); ++rule) {
        if (reachable.count(rule->first[0])) {
            clear_grammar.push_back(*rule);
        }
    }

    grammar = clear_grammar;
}

static void
print()
{
    for (auto rule = grammar.begin(); rule != grammar.end(); ++rule) {
        cout << rule->first << ' ' << rule->second << endl;
    }
}

int
main()
{
    init();
    calc();
    print();
}
