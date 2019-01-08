#include <cmath>

class ComplexNumber {
  double re, im;

public:
  constexpr
  ComplexNumber(double _re, double _im) noexcept :
    re(_re),
    im(_im)
  {
  }

  constexpr
  ComplexNumber() noexcept :
    re(0),
    im(0)
  {
  }

  constexpr double
  GetRe() const noexcept
  {
    return re;
  }

  constexpr double
  GetIm() const noexcept
  {
    return im;
  }

  constexpr void
  SetIm(double _im) noexcept
  {
    im = _im;
  }

  constexpr void
  SetRe(double _re) noexcept
  {
    re = _re;
  }
};

constexpr double EPS = 1e-5;

constexpr bool
operator==(const ComplexNumber &lhs, const ComplexNumber &rhs)
{
  return std::abs(lhs.GetRe() - rhs.GetRe()) <= EPS &&
         std::abs(lhs.GetIm() - rhs.GetIm()) <= EPS;
}

constexpr ComplexNumber Conjugate(const ComplexNumber& x) {
  ComplexNumber res;
  res.SetRe(x.GetRe());
  res.SetIm(-x.GetIm());
  return res;
}

int main() {
  constexpr ComplexNumber a(1, 2);
  constexpr ComplexNumber b(1, -2);
  constexpr auto c = Conjugate(a);
  static_assert(b == c, "failed");
}
