/*
    problem03-6 - recurrent sequence
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

    def K[i]:
      K[i] = i            <=> i <= 3
      K[i] = 2 * K[i + 3] <=> i - odd
      K[i] = 3 + K[i / 2] <=> i - even

    in >> n
    out << K[n]
*/

#include <stdio.h>
#include <inttypes.h>

uintmax_t
element_of_sequence (size_t i)
{
  if (i <= 3)
    return i;
  else if (i % 2 == 1)
    return 2 * element_of_sequence(i + 3);
  else // if (i%2 == 0)
    return 3 + element_of_sequence(i / 2);
}


int
main (void)
{
  uintmax_t n;
  scanf("%ju", &n);
  printf("%ju\n", element_of_sequence(n));
  return 0;
}

