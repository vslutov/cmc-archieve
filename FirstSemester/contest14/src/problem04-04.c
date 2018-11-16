/*
    problem04-04 - "A" and "B" letter
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

    Input string U which contain only "A" and "B" letter.
    Print all position x, there are:

      If delete letter with number x, amout of letter "A" in even and in odd
      positions are equal.
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

const size_t MAX_LENGTH = 255;

int
main (void)
{
  char s[MAX_LENGTH + 1];
  scanf("%s", s);

  size_t length = strlen(s);
  size_t even[length + 1], odd[length + 1];
  even[0] = odd[0] = 0;

  for (size_t i = 0; i < length; ++i)
    if (i % 2 == 0)
      {
        if (s[i] == 'A')
          even[i + 1] = even[i] + 1;
        else
          even[i + 1] = even[i];
        odd[i + 1] = odd[i];
      }
    else // i % 2 == 1
      {
        if (s[i] == 'A')
          odd[i + 1] = odd[i] + 1;
        else
          odd[i + 1] = odd[i];
        even[i + 1] = even[i];
      }

  for (size_t i = 1; i <= length; ++i)
    if ((even[i - 1] + (odd[length] - odd[i])) ==
        (odd[i - 1] + (even[length] - even[i])))
      printf("%zu ", i);

  printf("\n");

  return 0;
}

