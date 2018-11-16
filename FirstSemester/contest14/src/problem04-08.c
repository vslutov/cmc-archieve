/*
    problem04-08 - strikeout
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

    How many different three-digit numbers can be obtained from a given
    positive integer N, striken the figures of its decimal notation?
*/

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

const size_t MAX_LENGTH = 100;

int
main (void)
{
  char s[MAX_LENGTH + 1];
  scanf("%s", s);

  size_t length = strlen(s);

  bool exist[1000];
  for (size_t x = 0; x < 1000; ++x)
    exist[x] = false;

  for (size_t i = 0; i < length; ++i)
    for (size_t j = i + 1; j < length; ++j)
      for (size_t k = j + 1; k < length; ++k)
        {
          size_t a = s[i] - '0';
          size_t b = s[j] - '0';
          size_t c = s[k] - '0';

          size_t x = 100 * a + 10 * b + c;

          if (a != 0)
            exist[x] = true;
        }

  size_t count = 0;
  for (size_t x = 0; x < 1000; ++x)
    if (exist[x])
      ++ count;

  printf("%zu\n", count);

  return 0;
}

