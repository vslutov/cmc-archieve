/*
    problem04-05 - word rotate
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

    Rotate word for N position.
*/

#include <stdio.h>
#include <string.h>

const size_t MAX_LENGTH = 80;

int
main (void)
{
  size_t N;
  scanf("%zu\n", &N);

  char s[MAX_LENGTH + 1];
  scanf("%s", s);

  size_t length = strlen(s);
  for(size_t i = 0; i < length; ++i)
    putchar(s[(i + N) % length]);
  putchar('\n');

  return 0;
}

