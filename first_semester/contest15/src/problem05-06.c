/*
    problem05-06 - palindromes again
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

    String with length is equal L is palindrome?
*/

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int
main (void)
{
  size_t length;
  scanf("%zu\n", &length);

  char *string = (char *) malloc((length + 2));
  assert(string != NULL);

  fgets(string, length + 2, stdin);
  string[length + 1] = 0;

  bool is_palindrome = true;

  for (size_t i = 0; i < length / 2; ++i)
    if (string[i] != string[length - i - 1])
      {
        is_palindrome = false;
        break;
      }

  free(string);

  printf(is_palindrome ? "YES\n" : "NO\n");

  return 0;
}

