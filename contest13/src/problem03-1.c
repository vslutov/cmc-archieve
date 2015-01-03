/*
    problem03-1 - the greatest common divisor of three natural numbers
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

#include <stdio.h>
#include <inttypes.h>

uintmax_t
gcd (uintmax_t x, intmax_t y)
{
  if (y == 0)
    return x;
  else
    return gcd(y, x % y);
}

int
main (void)
{
  const size_t N = 4;
  uintmax_t current_answer, number;
  size_t i;

  scanf("%jd", &current_answer);
  for (i = 1; i < N; ++i)
    {
      scanf("%jd", &number);
      current_answer = gcd(current_answer, number);
    }

  printf("%jd\n", current_answer);
  return 0;
}

