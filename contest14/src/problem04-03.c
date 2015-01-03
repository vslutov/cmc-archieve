/*
    problem04-03 - string periods
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

    Input string A.
    Outputs all periods of it in strictly ascending sequence.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

const size_t MAX_LENGTH = 10 * 1000;

size_t *
P (const char *s)
{
  size_t length = strlen(s);

  size_t *pi;
  pi = (size_t *)malloc(length * sizeof(size_t));
  assert(pi != NULL);

  pi[0] = 0;

  size_t k = 0;
  for (size_t i = 1; i < length; ++i)
    {
      while (k > 0 && s[k] != s[i])
        k = pi[k - 1];

      if (s[k] == s[i])
        ++ k;

      pi[i] = k;
    }

  return pi;
}

int
main (void)
{
  char s[MAX_LENGTH + 1];

  gets(s);

  size_t length = strlen(s);
  size_t *pi = P(s);

  for (size_t current_position = pi[length - 1];
       current_position > 0;
       current_position = pi[current_position - 1])
    printf("%zu ", length - current_position);

  printf("%zu\n", length);

  free(pi);

  return 0;
}

