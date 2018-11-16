/*
    problem05-09 - virus
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

    Colony - table of cells.
    Virus infect surrounding cells.
    Output time to infect all colony by virus.
*/

#include <inttypes.h>
#include <stdio.h>

#include "point.template.c"
#include "queue.template.h"

QUEUE(Point)

int
main (void)
{
  size_t width, height;
  scanf("%zu %zu", &height, &width);

  intmax_t colony[width][height];
  for (size_t x = 0; x < width; ++x)
    for (size_t y = 0; y < height; ++y)
      colony[x][y] = -1;

  Queue_Point queue = new_queue_Point();

  size_t infected_cells;
  scanf("%zu", &infected_cells);

  for (size_t i = 0; i < infected_cells; ++i)
    {
      Point virus;
      scanf("%jd %jd", &virus.x, &virus.y);
      -- virus.x;
      -- virus.y;
      if (colony[virus.x][virus.y] == -1)
        {
          colony[virus.x][virus.y] = 0;
          push_back_queue_Point(queue, virus);
        }
    }

  Point steps[4];
  steps[0] = new_point(1, 0);
  steps[1] = new_point(-1, 0);
  steps[2] = new_point(0, 1);
  steps[3] = new_point(0, -1);

  while (!is_empty_queue_Point(queue))
  {
    Point current = pop_front_queue_Point(queue);
    for (size_t i = 0; i < 4; ++i)
      {
        Point next = sum_point(current, steps[i]);
        if (in_board(next, width, height) &&
            (colony[next.x][next.y] == -1))
          {
            colony[next.x][next.y] = colony[current.x][current.y] + 1;
            push_back_queue_Point(queue, next);
          }
      }
   }

  intmax_t time = 0;
  for (size_t x = 0; x < width; ++x)
    for (size_t y = 0; y < height; ++y)
      if (time < colony[x][y])
        time = colony[x][y];

  delete_queue_Point(queue);

  printf("%jd\n", time);

  return 0;
}

