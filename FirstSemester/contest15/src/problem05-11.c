/*
    problem05-11 - most cost road
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

    The road goes from edge to edge of matrix. (horizontal)
    Print out max cost.
*/

#include <stdio.h>
#include <inttypes.h>

int
main (void)
{
  size_t width, height;
  scanf("%zu %zu", &height, &width);

  uintmax_t space[height][width];
  for (size_t i = 0; i < height; ++ i)
    for (size_t j = 0; j < width; ++ j)
      scanf("%ju", &space[i][j]);

  uintmax_t dynamic[height][width];
  for (size_t i = 0; i < height; ++ i)
    dynamic[i][0] = space[i][0];

  for (size_t j = 1; j < width; ++ j)
    for (size_t i = 0; i < height; ++ i)
      {
        dynamic[i][j] = dynamic[i][j - 1];
        if ((i > 0) && (dynamic[i][j] < dynamic[i - 1][j - 1]))
          dynamic[i][j] = dynamic[i - 1][j - 1];
        if ((i < height - 1) && (dynamic[i][j] < dynamic[i + 1][j - 1]))
          dynamic[i][j] = dynamic[i + 1][j - 1];

        dynamic[i][j] += space[i][j];
      }

  uintmax_t result = 0;

  for (size_t i = 0; i < height; ++ i)
    if (result < dynamic[i][width - 1])
      result = dynamic[i][width - 1];

  printf("%ju\n", result);

  return 0;
}

