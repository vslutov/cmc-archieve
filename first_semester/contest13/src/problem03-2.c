/*
    problem03-2 - find P = the least prime greater or equal N
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
#include <stdbool.h>

bool
isprime (uintmax_t x)
{
  uintmax_t p_div; // possible divisor

  if (x == 1)
    return false;

  for (p_div = 2; p_div * p_div <= x; ++p_div)
    if (x % p_div == 0)
      return false;

  return true;
}

int
main (void)
{
  uintmax_t N;
  scanf("%ju", &N);

  while (!isprime(N))
    N ++;

  printf("%ju\n", N);
  return 0;
}

