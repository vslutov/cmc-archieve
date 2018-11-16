/*
    problem03-10 - sum of fractions
    Copyright (C) 2013  Lutov V. S.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
    Task description:

    Sum of rational number:
    a/b + a'/b' = (a*b' + a'*b)/(b*b')

    Calc sum of numbers without arrays:
    S = a[1]/b[1] + a[2]/b[2] + ... + a[n]/b[n]
*/

#include <stdio.h>
#include <inttypes.h>

struct Rational
{
  intmax_t numerator, denominator;
};

intmax_t
gcd (intmax_t a, intmax_t b)
{
  if (b == 0)
    return a;
  else
    return gcd(b, a%b);
}

struct Rational
add (struct Rational a, struct Rational b)
{
  struct Rational result;
  result.numerator = a.numerator * b.denominator + b.numerator * a.denominator;
  result.denominator = a.denominator * b.denominator;

  intmax_t factor = gcd(result.numerator, result.denominator);
  result.numerator /= factor;
  result.denominator /= factor;

  return result;
}

int
main (void)
{
  size_t n, i;
  scanf("%zu", &n);

  struct Rational sum, value;
  sum.numerator = 0;
  sum.denominator = 1;

  for (i = 0; i < n; ++i)
    {
      scanf("%jd %jd", &value.numerator, &value.denominator);
      sum = add(sum, value);
    }

  printf("%jd %jd %jd\n", sum.numerator / sum.denominator,
         sum.numerator % sum.denominator, sum.denominator);
  return 0;
}

