class C
{
public:
    C() {};
    C(int i) {};
    C(const C *c) {};
    C(double c, const C &v) {};

    C
    operator~() const
    {
        return *(new C);
    };

    C &
    operator++()
    {
        return *this;
    }

    int
    operator* (const C *a)
    {
        return 0;
    }

    operator int ()
    {
        return 0;
    }
};

double
operator+ (const C &c1, const C &c2)
{
    return 0;
};

C &
operator& (C &pc)
{
    return pc;
}

