#include <iostream>
#include <string>
#include <vector>

using namespace std;

class StringStack
{
private:
    string *values = new string[1];
    ssize_t real_size = 1;
    ssize_t virtual_size = 0;
public:
    void
    push(string str)
    {
        values[virtual_size] = str;
        ++ virtual_size;
        if (virtual_size == real_size) {
            string *temp = new string[real_size];
            for (ssize_t i = 0; i < real_size; ++i) {
                temp[i] = values[i];
            }
            delete[] values;
            values = new string[2 * real_size];
            for (ssize_t i = 0; i < real_size; ++i) {
                values[i] = temp[i];
            }
            delete[] temp;
            real_size *= 2;
        }
    };

    string
    pop()
    {
        auto result = values[virtual_size - 1];
        -- virtual_size;
        return result;
    }

    StringStack &
    operator=(string str)
    {
        virtual_size = 0;
        this->push(str);
        return *this;
    }

    StringStack &
    operator+=(string str)
    {
        this->push(str);
        return *this;
    }

    friend ostream &operator<<(ostream &, const StringStack &);
};

ostream &
operator<<(ostream &out, const StringStack &s)
{
    for (ssize_t i = s.virtual_size - 1; i >= 0; --i) {
        out << s.values[i] << endl;
    }

    return out;
}

StringStack &
operator,(StringStack &self, string str)
{
    self.push(str);
    return self;
}
