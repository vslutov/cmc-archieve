/*
    problem04-06 - paragraph formatting
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

    There are fixed symbols in every line.

    Note: there are every string end with space symbol in this program;
*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

const size_t MAX_LENGTH = 1000 * 1000;

void
print_line (const char *s, size_t length, size_t wide)
{
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

  if (space_series == 0)
    for (size_t j = 0; j < space_amount; ++j)
      putchar(' ');

  putchar('\n');
}

size_t
get_clipped_length (const char *s, size_t length, size_t wide)
{
  size_t last_space = 0;

  if (wide > length)
    return length;
  else
    {
      for (size_t i = 0; i < wide; ++i)
        if (s[i] == ' ')
          last_space = i;

      return last_space > 0 ? last_space : wide;
    }
}

void
print_paragraph (char *start, size_t length, size_t wide)
{
  while (length > 0)
  {
    size_t clipped_length;
    if ((wide <= length) && (start[wide] == ' '))
      clipped_length = wide;
    else
      clipped_length = get_clipped_length(start, length, wide);

    print_line(start, clipped_length, wide);

    length -= clipped_length;
    if (length > 0)
      -- length;
    start += clipped_length + 1;
  }
}

int
main (void)
{
  size_t wide;
  scanf("%zu\n", &wide);

  char input_string[MAX_LENGTH + 2];
  fgets(input_string, MAX_LENGTH + 2, stdin);

  size_t length = strlen(input_string);
  -- length;
  input_string[length] = '\0'; // replace '\n' with '\0'

  print_paragraph(input_string, length, wide);
  return 0;
}

