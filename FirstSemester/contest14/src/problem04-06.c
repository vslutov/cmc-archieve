/*
    problem04-06 - formatting
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

    Insert (K - length(s)) space between words.
*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

void
print_line (const char *s, size_t wide)
{
  size_t length = strlen(s);

  size_t space_series = 0;
  for (size_t i = 0; i < length; ++i)
    if (s[i] == ' ')
      ++ space_series;

  size_t space_amount = space_series + (wide - length);

  for (size_t i = 0; i < length; ++i)
    if (s[i] != ' ')
      putchar(s[i]);
    else
      {
        size_t series_length = space_amount / space_series;
        series_length += (space_amount % space_series != 0) ? 1 : 0;

        for (size_t j = 0; j < series_length; ++j)
          putchar(' ');

        space_amount -= series_length;
        -- space_series;
      }

  putchar('\n');

}

int
main (void)
{
  size_t wide;
  scanf("%zu\n", &wide);

  char s[wide + 2];
  fgets(s, wide + 2, stdin);

  size_t length = strlen(s);
  -- length;
  s[length] = 0; // replace '\n' with '\0'

  print_line(s, wide);

  return 0;
}

