/*
    problem03-12 - equal amount
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

    Find two nonempty subset with equal amount
*/


#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

intmax_t
max (intmax_t a, intmax_t b)
{
  if (a > b)
    return a;
  else
    return b;
}

intmax_t
max3 (intmax_t a, intmax_t b, intmax_t c)
{
  return max( max(a, b), c );
}

intmax_t
recursive_calc (intmax_t *a, size_t i, intmax_t sum_left,
                intmax_t sum_right, size_t n)
{
  if (i == n)
    if (sum_left == sum_right)
      return sum_left;
    else
      return 0;
  else
    return max3(recursive_calc(a, i + 1, sum_left + a[i], sum_right, n),
                recursive_calc(a, i + 1, sum_left, sum_right + a[i], n),
                recursive_calc(a, i + 1, sum_left, sum_right, n));
}

int
main (void)
{
  size_t n, i;
  scanf("%zu", &n);

  intmax_t a[n];
  for (i = 0; i < n; ++i)
    scanf("%jd", &a[i]);

  printf("%jd\n", recursive_calc(a, 0, 0, 0, n));
  return 0;
}

