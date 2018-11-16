/*
    problem03-17 - triple sequence
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

    A(1) = 1
    A(3 ** n + k) =
      if k < 3 ** n:
        A(3 ** n - k) + 3 ** n
      else:
        A(k - 3 ** n + 1) + 2 * 3 ** n
*/

#include <stdio.h>
#include <inttypes.h>

uintmax_t
A (uintmax_t n)
{
  if (n == 1)
    return 0;

  uintmax_t m = 1; // 3 ** k
  while (m * 3 < n)
    m *= 3;

  if (n <= 2*m)
    return A(2 * m - n + 1) + m;
  else
    return A(n - 2 * m) + 2 * m;
}

int
main (void)
{
  uintmax_t n;
  scanf("%ju", &n);
  printf("%ju\n", A(n));
  return 0;
}

