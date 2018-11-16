#include <iostream>
#include <vector>

using namespace std;

class Avg
{
    vector<double> values = vector<double>();
public:
    Avg &
    operator+=(double x)
    {
        values.push_back(x);
        return *this;
    };

    friend
    ostream &
    operator<<(ostream &, Avg &);
};

ostream &
operator<<(ostream &o, Avg &avg)
{
    double sum = 0;
    auto count = avg.values.size();
    for (auto x = avg.values.begin(); x != avg.values.end(); ++x) {
        sum += *x;
    }
    auto avr = sum / count;
    sum = 0;
    for (auto x = avg.values.begin(); x != avg.values.end(); ++x) {
        sum += (*x - avr) * (*x - avr);
    }
    o << avr << endl << sum / (count - 1) << endl;
    return o;
};
