/*
    problem05-14 - hyperprime
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

    N is hyperprime:
      N is prime
      N / 10 is prime or null
*/

#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

bool
is_prime (intmax_t n)
{
  if (n == 1)
    return false;
  for (size_t i = 2; i * i <= n; ++ i)
    if (n % i == 0)
      return false;
  return true;
}

void
recursive_find (intmax_t n, size_t place, size_t length)
{
  if (is_prime(n))
    {
      if (place == length)
        printf("%jd ", n);
      else
        for (intmax_t i = 1; i < 10; ++ i)
          recursive_find(n * 10 + i, place + 1, length);
    }
}

int
main (void)
{
  size_t n;
  scanf("%zu", &n);

  recursive_find(0, 0, n);
  putchar('\n');

  return 0;
}

