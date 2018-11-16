#include <iostream>
#include <ios>
#include <cmath>
#include <vector>

namespace Numbers {
    const double EPS = 1e-8;

    class Complex {
    private:
        double re, im;
    public:
        Complex(double r=0.0, double i=0.0) : re(r), im(i) {}

        double
        get_re() const;

        double
        get_im() const;

        double
        abs2() const;

        double
        abs() const;

        friend std::istream &
        operator>>(std::istream &, Complex &);

        operator bool() const;
    };

    inline double
    Complex::get_re() const
    {
        return this->re;
    }


    inline double
    Complex::get_im() const
    {
        return this->im;
    }

    inline double
    Complex::abs2() const
    {
        return re * re + im * im;
    }

    inline double
    Complex::abs() const
    {
        return std::sqrt(this->abs2());
    }


    inline Complex
    operator+(const Complex &a, const Complex &b)
    {
        return Complex(a.get_re() + b.get_re(), a.get_im() + b.get_im());
    }

    inline Complex
    operator*(const Complex &a, const Complex &b)
    {
        return Complex(a.get_re() * b.get_re() - a.get_im() * b.get_im(),
                       a.get_im() * b.get_re() + a.get_re() * b.get_im());
    }

    inline Complex
    operator+(const Complex &a)
    {
        return a;
    }

    inline Complex
    operator-(const Complex &a)
    {
        return Complex(-a.get_re(), -a.get_im());
    }

    inline Complex
    operator-(const Complex &a, const Complex &b)
    {
        return a + (-b);
    }

    inline std::ostream &
    operator<<(std::ostream &out, const Complex &a)
    {
        auto flags = out.flags();
        out.precision(10);
        out << std::scientific << a.get_re() << " " << a.get_im();
        out.flags(flags);
        return out;
    };

    inline std::istream &
    operator>>(std::istream &in, Complex &a)
    {
        return in >> a.re >> a.im;
    };

    inline
    Complex::operator bool() const
    {
        return std::abs(this->get_re()) + std::abs(this->get_im()) > EPS;
    }

    inline bool
    operator!(const Complex &a)
    {
        return !bool(a);
    }

    inline double
    abs(const Complex &a)
    {
        return a.abs();
    }

    inline std::vector<Complex>
    sqrt(const Complex &a)
    {
        auto result = std::vector<Complex>(2);
        const auto phi = atan2(a.get_im(), a.get_re()) / 2;
        std::cout << phi << std::endl;
        const auto r = std::sqrt(a.abs());
        result[0] = Complex(r * cos(phi), r * sin(phi));
        result[1] = Complex(r * cos(phi + M_PI), r * sin(phi + M_PI));
        return result;
    }
}

namespace Equations {
    int
    quadratic(const std::vector<Numbers::Complex> &v,
              std::vector<Numbers::Complex> &result)
    {
        result.clear();
        const auto a = v[2];
        const auto b = v[1];
        const auto c = v[0];

        if (!a && !b && !c) {
            return -1;
        } else if (!a && !b && c) {
            return 0;
        } else if (!a) {
            result.push_back(-c / b);
            return 1;
        } else {
            const auto D = b * b - Numbers::Complex(4) * a * c;
            const auto sqrt_D = Numbers::sqrt(D);
            result.push_back((-b + sqrt_D[0]) / (Numbers::Complex(2) * a));
            result.push_back((-b + sqrt_D[1]) / (Numbers::Complex(2) * a));
            return 2;
        }
    }
}

