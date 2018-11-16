/*
    point - hand-made point
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

#include <inttypes.h>
#include <stdbool.h>

typedef struct
{
  intmax_t x, y;
} Point;

Point
new_point (intmax_t x, intmax_t y)
{
  Point result = {.x = x, .y = y};
  return result;
}

Point
sum_point (Point a, Point b)
{
  Point result = {.x = a.x + b.x, .y = a.y + b.y};
  return result;
}

bool
in_board (Point a, intmax_t width, intmax_t height)
{
  return (a.x >= 0) && (a.y >= 0) && (a.x < width) && (a.y < height);
}

