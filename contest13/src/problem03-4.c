/*
    problem03-4 - magic sequence reverse
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
#include <stdbool.h>
#include <inttypes.h>

void
recursive_magic (bool even)
{
  // even = true => now we read element with even number
  // even = false => now we read element with odd number
  intmax_t element;
  scanf("%jd", &element);
  if (element != 0)
  {
    if (!even)
      {
        // first output odd elements
        printf("%jd ", element);
        recursive_magic(!even);
      }
    else
      {
        // then output even elements in reverse order
        recursive_magic(!even);
        printf("%jd ", element);
      }
  }
}

int
main (void)
{
  recursive_magic(false);
  printf("\n");
  return 0;
}

