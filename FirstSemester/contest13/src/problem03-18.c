/*
    problem03-18 - double sequence
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

    A#B = AB (10#20 = 1020)

    in >> N, A
    X(1) = N mod 2010
    x(i+1) = X(i)#X(i) mod 2010

    A in X(1..N)?
*/

#include <stdio.h>
#include <inttypes.h>

uintmax_t
sharp (uintmax_t A, uintmax_t B)
{
  uintmax_t copy_B = B;
  while (B != 0)
  {
    A *= 10;
    B /= 10;
  }

  A += copy_B;

  return A;
}

int
main (void)
{
  const uintmax_t VALUE_AREA = 2010;
  uintmax_t N, A, X, i;
  scanf("%ju %ju", &N, &A);
  X = N % VALUE_AREA;
  for (i = 0; i < VALUE_AREA; ++i)
    {
      if (X == A)
        {
          printf("YES\n");
          return 0;
        }
      X = sharp(X, X) % VALUE_AREA;
    }
  printf("NO\n");
  return 0;
}

