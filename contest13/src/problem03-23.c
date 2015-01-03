/*
    problem03-23 - regular pyramid
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

    Square = X * X
    Level = N * equal square

    Pyramid = K * level:
      count_of_square(level) = count_of_square(level - 1) - 1
      length(level) < length(level - 1)
      count_of_square(K) = 1

    Can we build pyramid from current squares?
*/

#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>

struct Squares
{
  intmax_t size, count;
};

struct Squares
scan_squares (void)
{
  struct Squares result;
  scanf("%jd %jd", &result.size, &result.count);
  return result;
}

bool
recursive_test(struct Squares *squares, size_t N, intmax_t level,
               intmax_t last_length)
{
  size_t i;
  bool has_any_squares = false;
  for (i = 0; i < N; ++i)
    if (squares[i].count != 0)
      {
        if ((squares[i].count * squares[i].size <= last_length) ||
            (squares[i].count < level))
          return false;
        else
          {
            has_any_squares = true;
            squares[i].count -= level;
            if (recursive_test(squares, N, level + 1, level * squares[i].size))
              return true;
            squares[i].count += level;
          }
      }

  return !has_any_squares;
}

int
main (void)
{
  size_t N;
  scanf("%zu", &N);

  struct Squares squares[N];
  size_t i;
  for (i = 0; i < N; ++i)
    squares[i] = scan_squares();

  if (recursive_test(squares, N, 1, 0))
    printf("Yes\n");
  else
    printf("No\n");

  return 0;
}

