/*
    problem03-5 - palindrome on integer value
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

    P(t) - reverse function for decimal notation of integer
    in >> x
    in >> number_of_step
    do number_of_step times:
      x = x + P(x)
    now x is palindrome?
*/

#include <stdio.h>
#include <inttypes.h>

uintmax_t
recursive_reverse_integer (uintmax_t x, uintmax_t buffer)
{
  if (x == 0)
    return buffer;
  else
    // move the least digit to the buffer
    // 123, 0 -> 12, 3 -> 1, 32 -> 0, 321
    return recursive_reverse_integer(x / 10, buffer * 10 + (x % 10));
}

uintmax_t
reverse_integer (uintmax_t x)
{
  return recursive_reverse_integer(x, 0);
}

int
main (void)
{
  uintmax_t x;
  size_t number_of_steps, i;

  scanf("%ju %zu",  &x, &number_of_steps);

  for (i = 0; i < number_of_steps; ++i)
    x = x + reverse_integer(x);

  if (x == reverse_integer(x))
    printf("Yes\n%ju\n", x);
  else
    printf("No\n");

  return 0;
}

