/*
    problem05-13 - dijkstra
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

    Print out the total change of the height during the passage of the optimal
    route.
*/

#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

#include "point.template.c"
#include "treap.template.h"

typedef struct
{
  Point place;
  intmax_t time;
} TimePlace;

bool
less_time (TimePlace a, TimePlace b)
{
  if ((a.time < b.time) ||
      ((a.time == b.time) &&
       ((a.place.x < b.place.x) ||
        ((a.place.x == b.place.x) && (a.place.y < b.place.y)))))
    return true;
  else
    return false;
}

TREAP(TimePlace, less_time)

int
main (void)
{
  Point size, finish;
  TimePlace start;

  scanf("%jd %jd %jd %jd %jd %jd", &size.x, &size.y, &start.place.x,
        &start.place.y, &finish.x, &finish.y);

  start.time = 0;

  intmax_t place[size.x][size.y];
  intmax_t time[size.x][size.y];

  for (size_t x = 0; x < size.x; ++ x)
    for (size_t y = 0; y < size.y; ++ y)
    {
      scanf("%jd", &place[x][y]);
      time[x][y] = -1;
    }

  Treap_TimePlace treap = new_treap_TimePlace();
  insert_treap_TimePlace(treap, start);

  Point steps[4];
  steps[0] = new_point(0, 1);
  steps[1] = new_point(0, -1);
  steps[2] = new_point(1, 0);
  steps[3] = new_point(-1, 0);

  while (!is_empty_treap_TimePlace(treap))
    {
      TimePlace current = min_treap_TimePlace(treap);
      remove_treap_TimePlace(treap, current);

      if (time[current.place.x][current.place.y] != -1)
        continue;

      time[current.place.x][current.place.y] = current.time;

      if (current.place.x == finish.x && current.place.y == finish.y)
        break;

      for (size_t i = 0; i < 4; ++ i)
        {
          TimePlace next;
          next.place = sum_point(current.place, steps[i]);
          if (in_board(next.place, size.x, size.y) &&
              time[next.place.x][next.place.y] == -1)
            {
              next.time = current.time +
                          abs(+ place[current.place.x][current.place.y]
                              - place[next.place.x][next.place.y]);
              insert_treap_TimePlace(treap, next);
            }
        }
    }

  delete_treap_TimePlace(treap);

  printf("%jd\n", time[finish.x][finish.y]);

  return 0;
}

