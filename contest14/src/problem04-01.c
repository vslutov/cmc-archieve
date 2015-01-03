/*
    problem04-01 - pefix and suffix
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

    Input strings A and B.
    Outputs P(A + B) and P(B + A).
    Where P(X) is prefix-function.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

const size_t MAX_LENGTH = 10 * 1000;

size_t
P (const char *s)
{
  size_t length = strlen(s);

  size_t *p;
  p = malloc(length * sizeof(size_t));
  assert(p != NULL);

  p[0] = 0;

  size_t k = 0;
  for (size_t i = 1; i < length; ++i)
    {
      while (k > 0 && s[k] != s[i])
        k = p[k - 1];

      if (s[k] == s[i])
         ++ k;

      p[i] = k;
    }

  free(p);
  return k;
}

int
main (void)
{
  const char *separator = " ";

  char a[MAX_LENGTH + 1], b[MAX_LENGTH + 1];
  char sum[2 * (MAX_LENGTH + 1) + 1];

  gets(a);
  gets(b);

  strcpy(sum, a);
  strcat(sum, separator);
  strcat(sum, b);
  printf("%zu ", P(sum));

  strcpy(sum, b);
  strcat(sum, separator);
  strcat(sum, a);
  printf("%zu\n", P(sum));

  return 0;
}

