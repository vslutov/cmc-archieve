/*
    problem05-10 - long arithmetic
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

    Implement long addition, substraction and multiply.
*/

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

const size_t MAX_LENGTH = 10 * 1000;

char *
add (const char *a, const char *b)
{
  size_t length_a = strlen(a);
  size_t length_b = strlen(b);

  size_t length = ((length_a >= length_b) ? length_a : length_b) + 1;

  char *result = (char *) malloc(length + 1);

  intmax_t shift = 0;
  for (size_t i = 0; i < length; ++i)
    {
      intmax_t x = i < length_a ? a[length_a - i - 1] - '0' : 0;
      intmax_t y = i < length_b ? b[length_b - i - 1] - '0' : 0;

      intmax_t r = x + y + shift;
      shift = r / 10;
      r = r % 10;

      result[length - i - 1] = '0' + r;
    }

  result[length] = 0;

  if (result[0] == '0')
    for (size_t i = 0; i < length; ++i)
      result[i] = result[i + 1];

  return result;
}

char *
substract (const char *a, const char *b)
{
  size_t length_a = strlen(a);
  size_t length_b = strlen(b);

  size_t length = ((length_a >= length_b) ? length_a : length_b) + 1;

  char *result = (char *) malloc(length + 1);

  intmax_t shift = 0;
  for (size_t i = 0; i < length; ++i)
    {
      intmax_t x = i < length_a ? a[length_a - i - 1] - '0' : 0;
      intmax_t y = i < length_b ? b[length_b - i - 1] - '0' : 0;

      intmax_t r = x - y + shift;
      shift = r / 10;
      r = r % 10;

      result[length - i - 1] = '0' + r;
    }

  result[length] = 0;

  size_t start = 0;
  while (result[start] == '0')
    ++ start;

  bool negative;

  if (result[start] == 0)
    {
      result = realloc(result, 2);
      assert(result != NULL);

      strcpy(result, "0");
      return result;
    }
  else if (result[start] > '0')
    negative = false;
  else // result < 0
    {
      negative = true;
      for (size_t i = start; i < length; ++i)
        {
          intmax_t x = result[i] - '0';
          x *= -1;
          result[i] = '0' + x;
        }
    }

  for (size_t i = start + 1; i < length; ++ i)
    if (result[i] < '0')
      {
        size_t j = i - 1;
        while (result[j] == '0')
          -- j;
        -- result[j];
        for (size_t k = j + 1; k < i; ++ k)
          result[k] = '9';
        result[i] += 10;
      }

  if (result[start] == '0')
    ++ start;

  if (negative)
    {
      result[0] = '-';
      for (size_t i = start; i <= length; ++ i)
        result[i - start + 1] = result[i];
    }
  else
    for (size_t i = start; i <= length; ++ i)
      result[i - start] = result[i];

  return result;
}

char *
multiply (const char *a, const char *b)
{
  if ((strcmp(a, "0") == 0) || (strcmp(b, "0") == 0))
    {
      char *result = (char *) malloc(2);
      strcpy(result, "0");
      return result;
    }

  size_t length_a = strlen(a);
  size_t length_b = strlen(b);

  size_t length = length_a + length_b;

  char *result = (char *) malloc(length + 1);

  intmax_t shift = 0;
  for (size_t i = 0; i < length; ++ i)
    {
      size_t r = shift;
      for (size_t j = 0; j < length_a; ++ j)
        if ((j <= i) && (i < j + length_b))
          {
            intmax_t x = a[length_a - j - 1] - '0';
            intmax_t y = b[length_b - (i - j) - 1] - '0';
            r += x * y;
          }

      shift = r / 10;
      r = r % 10;

      result[length - i - 1] = r + '0';
    }

  result[length] = 0;
  if (result[0] == '0')
    for (size_t i = 0; i < length; ++i)
      result[i] = result[i + 1];

  return result;
}

int
main (void)
{
  char *a, *b;
  char operation;

  char expression[MAX_LENGTH * 2 + 3];
  fgets(expression, MAX_LENGTH * 2 + 3, stdin);
  if (expression[strlen(expression) - 1] == '\n')
    expression[strlen(expression) - 1] = 0;

  size_t i = 0;
  while (('0' <= expression[i]) && (expression[i] <= '9'))
    ++ i;

  operation = expression[i];
  expression[i] = 0;
  a = expression;
  b = expression + i + 1;

  char *result;

  if (operation == '+')
    result = add(a, b);
  else if (operation == '-')
    result = substract(a, b);
  else
    result = multiply(a, b);

  printf("%s\n", result);
  free(result);

  return 0;
}

