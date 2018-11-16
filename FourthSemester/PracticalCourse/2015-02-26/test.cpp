#include <iostream>
#include <memory>
#include <string>
using namespace std;

struct Formula
{
    virtual explicit operator double() const = 0;
    virtual ~Formula() {};
    virtual string toString() const = 0;
};

typedef const shared_ptr<const Formula> FormulaPtr;

struct VarStruct : Formula
{
    double value;
    VarStruct(double val) : value(val) {}

    explicit
    operator double() const
    {
        return value;
    }

    string
    toString() const
    {
        return to_string(value);
    }
};

typedef const struct ValC : shared_ptr<const VarStruct>
{
    ValC(double val) : shared_ptr<const VarStruct>(new VarStruct(val)) {}

    ValC(const ValC &v) delete;
} Val;


struct Var : shared_ptr<VarStruct>
{
    explicit Var(double val) : shared_ptr<VarStruct>(new VarStruct(val)) {}

    Var&
    operator= (double v)
    {
        (*this)->value = v;
        return *this;
    }
};

struct Sum : Formula
{
    FormulaPtr a, b;

    Sum(FormulaPtr &x, FormulaPtr &y) : a(x), b(y) {};

    explicit
    operator double() const
    {
        return double(*a) + double(*b);
    };

    string
    toString() const
    {
        return "(" + a->toString() + " + " + b->toString() + ")";
    }
};

FormulaPtr operator+(FormulaPtr &x, FormulaPtr &y)
{
    return FormulaPtr(new Sum(x, y));
};

struct Mul : Formula
{
    FormulaPtr a, b;

    Mul(FormulaPtr &x, FormulaPtr &y) : a(x), b(y) {};

    explicit operator double() const {
        return double(*a) * double(*b);
    };

    string
    toString() const
    {
        return "(" + a->toString() + " * " + b->toString() + ")";
    }
};

FormulaPtr operator*(FormulaPtr &x, FormulaPtr &y)
{
    return FormulaPtr(new Mul(x, y));
}

int
main()
{
    auto x = Val(5);
    auto y = Val(1) + x + x * x;
    cout << y->toString() << " = " << double(*y) << endl;
    x = 4;
    cout << y->toString() << " = " << double(*y) << endl;
}
