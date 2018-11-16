#include <cinttypes>

class Sum
{
    intmax_t value;
public:
    Sum(intmax_t a, intmax_t b) : value(a + b) {}
    intmax_t get()
    {
        return value;
    }
};
