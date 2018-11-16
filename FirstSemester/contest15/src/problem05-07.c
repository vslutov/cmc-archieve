/*
    problem05-07 - string sort
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

    Sort string lexigraphically.
    Lowercase letter less than uppercase.
    Letter 'A' less then 'Z'.
*/

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef char *String;

bool
less_char (char a, char b)
{
  if ((islower(a) && isupper(b)) ||
      (islower(a) && islower(b) && (a < b)) ||
      (isupper(a) && isupper(b) && (a < b)) ||
      ((a == 0) && (b != 0)))
    return true;
  else
    return false;
}

bool
less_string (const String a, const String b)
{
  size_t i = 0;
  while ((a[i] != 0) && (b[i] != 0) && (a[i] == b[i]))
    ++ i;

  return less_char(a[i], b[i]);
}

int
compar (const void *a, const void *b)
{
  const String str_a = *((String *)a);
  const String str_b = *((String *)b);

  if (less_string(str_a, str_b))
    return -1;
  else if (less_string(str_b, str_a))
    return +1;
  else
    return 0;
}

const size_t MAX_LENGTH = 9 * 1000 * 1000;

int
main (void)
{
  size_t n;
  scanf("%zu\n", &n);

  String string = (String) malloc(MAX_LENGTH + 2);
  String array[n];

  for (size_t i = 0; i < n; ++ i)
    {
      fgets(string, MAX_LENGTH + 2, stdin);
      size_t length = strlen(string);
      if (string[length - 1] == '\n')
        string[-- length] = 0;

      array[i] = (String) malloc(length + 1);
      assert(array[i] != NULL);

      strcpy(array[i], string);
    }

  free(string);

  qsort(array, n, sizeof(string), compar);

  for (size_t i = 0; i < n; ++ i)
    {
      puts(array[i]);
      free(array[i]);
    }

  return 0;
}

