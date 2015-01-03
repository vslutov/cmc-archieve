/*
    problem03-15 - polygon
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

    Test good polygon for OY symmetry.
    Don't use arrays and cycles!
*/

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>

struct Vector
{
  intmax_t x, y;
};

bool
recursive_test (size_t i, struct Vector *vertex, size_t N)
{
  if (i == 2*N)
    return true;
  else
    {
      intmax_t x, y;
      scanf("%jd %jd", &x, &y);
      if (i <= N)
        {
          vertex->x = x;
          vertex->y = y;
          return recursive_test(++i, ++vertex, N);
        }
      else
        {
          struct Vector *symmetry_vertex = vertex - 2 * (i - N);
          if ((symmetry_vertex->x == -x) &&
              (symmetry_vertex->y == y))
            return recursive_test(++i, ++vertex, N);
          else
            return false;
        }
    }
}

int
main (void)
{
  size_t N;
  scanf("%zu", &N);
  N /= 2;
  struct Vector *polygon;
  polygon = (struct Vector*) malloc((N + 1) * sizeof(struct Vector));

  if (recursive_test(0, polygon, N))
    printf("Yes\n");
  else
    printf("No\n");

  free(polygon);

  return 0;
}

