/*
    problem03-20 - antifunction
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

    Build function g(x):

      for every x from f(t)
      f(g(x)) = x

      x - unsigned
      (maybe overflow)

      def f(t):
        s = 0
        i = 0
        while s <= x:
          s += 2*i + 1
          ++ i

        # s = sum(0, i, 2*i + 1) = (i+1) ** 2

        return unsigned(x * (i - 1))

        # x * (i-1) = x * floor(sqrt(x))

    0. M(log(n)) = O(32)
    1. M(f(x)) = O(1)
    2. We can use bin_search [O(log(n) * M(f(x)))] for fast finding.
    3. We should do exhaustive search for overflow [2 ** 16 * M(bin_search)]
    4. M(task) = 2**16 * log(n) = 2**21 = 10**7 - it's not TL!

*/

#include <stdio.h>
#include <math.h>

long long
f (long long x)
{
  return x * floor(sqrt(x));
}

long long
bin_search (long long x)
{
  long long m1 = 0, m2 = ((long long)1 << 32), m;
  while(m2 - m1 > 1)
  {
    m = (m1 + m2) / 2;
    if(f(m) <= x)
        m1 = m;
    else
        m2 = m;
  }
  return m1;
}

unsigned g(long long x)
{
  long long h = bin_search(x);
  if(f(h) == x)
    return h;
  int k = 0;
  while(!k)
  {
    x += (long long)1 << 32;
    h = bin_search(x);
    if(f(h) == x)
      k = 1;
  }
  return h;
}

int
main (void)
{
  unsigned y;

  scanf("%u", &y);
  printf("%u\n", g(y));

  return 0;
}

