/*
    problem03-8 - median iteration
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
    Algorithm:

    in >> a[n]
    n is odd

    do n // 2 times:
      for i in [1..n-2]:
        b[i] = median(a[i], a[i+1], a[i+2])
      a = b
      n = n-2

    out << a
*/

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

const size_t median_count = 3;

intmax_t
median(const intmax_t *a)
{
  intmax_t median_array[median_count];
  intmax_t t;
  size_t i, j;
  for (i = 0; i < median_count; ++i)
    median_array[i] = a[i];

  for (i = 0; i < median_count; ++i)
    for (j = i + 1; j < median_count; ++j)
      if (median_array[i] > median_array[j])
        {
          t = median_array[i];
          median_array[i] = median_array[j];
          median_array[j] = t;
        }

  return median_array[median_count / 2];
}

int
main (void)
{
  size_t n, i;
  scanf("%zu", &n);
  intmax_t *a = malloc(n * sizeof(intmax_t));
  intmax_t *b = malloc(n * sizeof(intmax_t));
  intmax_t *c;

  for (i = 0; i < n; ++i)
    scanf("%jd", &a[i]);

  while (n > 1)
  {
    for (i = 0; i < n - (median_count - 1); ++i)
    {
      b[i] = median(a + i);
    }

    c = a;
    a = b;
    b = c;

    n -= median_count - 1;
  }

  printf("%jd\n", a[0]);

  free(a);
  free(b);

  return 0;
}

