/*
    problem05-05 - string concat
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

    Output concat of three string [s3 + s1 + s2].
*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int
main (void)
{
  size_t length;
  scanf("%zu\n", &length);

  typedef char *string;
  string s[3];


  for (size_t i = 0; i < 3; ++i)
    {
      s[i] = (char *) malloc(length + 2);
      assert(s != NULL);

      fgets(s[i], length + 2, stdin);
      s[i][length] = 0;

    }

  fputs(s[2], stdout);
  fputs(s[0], stdout);
  fputs(s[1], stdout);
  putc('\n', stdout);

  for (size_t i = 0; i < 3; ++i)
    free(s[i]);

  return 0;
}

