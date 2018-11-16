/*
    problem04-12 - page formatting
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

    There are fixed symbols count in every line.
    There are fixed line count in every colon.
    There are fixed colon count in every page.
*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

const size_t MAX_LENGTH = 1000 * 1000;

typedef struct
{
  const char *start;
  size_t length;
} Line;

void
print_line (const char *s, size_t length, size_t wide)
{
  if (length > 0)
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
    }
}

size_t
save_line (const char *start, size_t length, size_t wide, Line *line)
{
  size_t line_length;

  if ((wide <= length) && (start[wide] == ' '))
    line_length = wide;
  else if (wide > length)
    line_length = length;
  else
    {
      size_t last_space = 0;
      for (size_t i = 0; i < wide; ++i)
        if (start[i] == ' ')
          last_space = i;

      line_length = last_space > 0 ? last_space : wide;
    }

  line->start = start;
  line->length = line_length;

  return line_length;
}

void
move_cursor (const char **start, size_t *length, size_t step)
{
  *length -= step;
  if (*length > 0)
    -- (*length);

  *start += step;
  if (step > 0)
    ++ (*start);
}

size_t
save_colon (const char *start, size_t length, size_t wide, size_t line_amount,
            Line colon[line_amount])
{
  const char *colon_start = start;

  for (size_t line_iterator = 0; line_iterator < line_amount; ++line_iterator)
    {
      size_t line_length = save_line(start, length, wide,
                                     colon + line_iterator);

      move_cursor(&start, &length, line_length);
    }

  return start - colon_start - 1;
}

size_t
print_page (const char *start, size_t length, size_t wide, size_t colon_amount,
            size_t line_amount)
{
  Line page[colon_amount][line_amount];

  const char *page_start = start;
  size_t last_colon = -1;

  for (size_t colon_iterator = 0;
       colon_iterator < colon_amount;
       ++ colon_iterator)
    if (length > 0)
      {
        size_t colon_length = save_colon(start, length, wide, line_amount,
                                         page[colon_iterator]);

        last_colon = colon_iterator;
        move_cursor(&start, &length, colon_length);
      }
    else
      break;

  for (size_t line_iterator = 0;
       line_iterator < line_amount;
       ++ line_iterator)
    {
      for (size_t colon_iterator = 0;
           colon_iterator < last_colon;
           ++ colon_iterator)
        {
          print_line(page[colon_iterator][line_iterator].start,
                     page[colon_iterator][line_iterator].length,
                     wide);
          printf("    ");
        }

      print_line(page[last_colon][line_iterator].start,
                 page[last_colon][line_iterator].length,
                 wide);
      printf("\n");
    }

  printf("\n\n");

  return start - page_start - 1;
}

void
print_file (const char *start, size_t length, size_t wide, size_t colon_amount,
            size_t line_amount)
{
  while (length > 0)
  {
    size_t page_length = print_page(start, length, wide, colon_amount,
                                    line_amount);

    move_cursor(&start, &length, page_length);
  }
}

int
main (void)
{
  size_t wide, colon_amount, line_amount;
  scanf("%zu %zu %zu\n", &wide, &colon_amount, &line_amount);

  char input_string[MAX_LENGTH + 2]; // space for '\n' and '\0'
  fgets(input_string, MAX_LENGTH + 2, stdin);

  size_t length = strlen(input_string);
  -- length;
  input_string[length] = '\0'; // replace '\n' with '\0'

  print_file(input_string, length, wide, colon_amount, line_amount);
  return 0;
}
