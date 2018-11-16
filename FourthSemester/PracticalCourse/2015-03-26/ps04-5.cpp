template <int x>
class Fib
{
public:
    static const int VAL = Fib<x-2>::VAL + Fib<x-1>::VAL;
};

template <>
class Fib <0>
{
public:
    static const int VAL = 0;
};

template <>
class Fib <1>
{
public:
    static const int VAL = 1;
};
