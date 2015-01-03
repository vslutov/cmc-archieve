/*
    problem05-12 - linear system
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

    Output solution of linear equation system.
*/

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

#include"vector.template.h"

VECTOR(size_t)

intmax_t
gcd (intmax_t a, intmax_t b)
{
  return b == 0 ? a : gcd(b, a % b);
}

intmax_t
Abs (intmax_t a)
{
  return a < 0 ? -a : a;
}

int
main (void)
{
  size_t n;
  scanf("%zu", &n);

  intmax_t a[n][n];
  intmax_t b[n];
  bool used[n];
  Vector_size_t finded = new_vector_size_t();

  for (size_t i = 0; i < n; ++ i)
    {
      for (size_t j = 0; j < n; ++ j)
        scanf("%jd", &a[i][j]);
      scanf("%jd", &b[i]);
    }

  for (size_t i = 0; i < n; ++ i)
    used[i] = false;

  for (size_t i = 0; i < n; ++ i)
    {
      size_t k = n;
      for (size_t x = 0; x < n; ++ x)
        if (!used[x] && (a[x][i] != 0) &&
            ((k == n) || (Abs(a[k][i]) > Abs(a[x][i]))))
          k = x;

      used[k] = true;
      push_back_vector_size_t(finded, k);

      for (size_t x = 0; x < n; ++ x)
        if (!used[x] && (a[x][i] != 0))
          {
            intmax_t first_gcd = gcd(a[k][i], a[x][i]);
            intmax_t multiplicator_k = a[x][i] / first_gcd;
            intmax_t multiplicator_x = a[k][i] / first_gcd;

            for (size_t j = i; j < n; ++ j)
              a[x][j] = a[x][j] * multiplicator_x - a[k][j] * multiplicator_k;
            b[x] = b[x] * multiplicator_x - b[k] * multiplicator_k;
          }
    }

  intmax_t result[n];
  for (size_t i = 0; i < n; ++ i)
    {
      size_t k = pop_back_vector_size_t(finded);

      intmax_t res = b[k];
      for (size_t j = n - i; j < n; ++ j)
        res -= a[k][j] * result[j];
      res /= a[k][n - i - 1];

      result[n - i - 1] = res;
    }

  delete_vector_size_t(finded);

  for (size_t i = 0; i < n; ++ i)
    printf("%jd\n", result[i]);

  return 0;
}

