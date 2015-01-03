/*
    problem05-08 - chess knight
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

    How many steps is needed for chess knight to go from point A to point B?
*/

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

#include "point.template.c"
#include "queue.template.h"

QUEUE (Point)

intmax_t
bfs (Point a, Point b, size_t size)
{
  a = sum_point(a, new_point(-1, -1));
  b = sum_point(b, new_point(-1, -1));

  intmax_t board[size][size];

  for (size_t x = 0; x < size; ++x)
    for (size_t y = 0; y < size; ++y)
      board[x][y] = -1;

  const size_t step_amount = 8;
  Point steps[step_amount];
  steps[0] = new_point(1, 2);
  steps[1] = new_point(-1, 2);
  steps[2] = new_point(1, -2);
  steps[3] = new_point(-1, -2);
  steps[4] = new_point(2, 1);
  steps[5] = new_point(-2, 1);
  steps[6] = new_point(2, -1);
  steps[7] = new_point(-2, -1);

  Queue_Point queue = new_queue_Point();

  board[a.x][a.y] = 0;
  push_back_queue_Point(queue, a);

  while (!is_empty_queue_Point(queue))
  {
    Point pos = pop_front_queue_Point(queue);
    for (size_t i = 0; i < step_amount; ++i)
    {
      Point next = sum_point(pos, steps[i]);
      if (in_board(next, size, size) && (board[next.x][next.y] == -1))
        {
          board[next.x][next.y] = board[pos.x][pos.y] + 1;
          push_back_queue_Point(queue, next);

          if ((next.x == b.x) && (next.y == b.y))
            break;
        }
    }
  }

  intmax_t result = board[b.x][b.y];

  delete_queue_Point(queue);

  return result;
}

int
main (void)
{
  size_t size;
  scanf("%zu", &size);

  Point a, b;
  scanf("%jd %jd %jd %jd", &a.x, &a.y, &b.x, &b.y);

  printf("%jd\n", bfs(a, b, size));

  return 0;
}

