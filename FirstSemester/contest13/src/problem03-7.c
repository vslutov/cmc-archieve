/*
    problem03-7 - the largest triangle
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

    Find the largest triangle stretched onto points
*/

#include <stdio.h>
#include <math.h>

const double EPS = 1e-9;

struct Vector
{
  double x, y;
};

double
max (double a, double b)
{
  if (a >= b - EPS) // a >= b
    return a;
  else
    return b;
}

struct Vector
sub (struct Vector A, struct Vector B)
{
  struct Vector AB;
  AB.x = B.x - A.x;
  AB.y = B.y - A.y;
  return AB;
}

double
exterior_product (struct Vector V, struct Vector U)
{
  return V.x * U.y - V.y * U.x;
}

double
area (struct Vector A, struct Vector B, struct Vector C)
{
  struct Vector AB = sub(A, B);
  struct Vector BC = sub(B, C);
  return fabs(exterior_product(AB, BC) / 2.0);
}

int
main (void)
{
  size_t n, i, j, k;
  double max_area = 0;

  scanf("%zu", &n);
  struct Vector points[n];

  for (i = 0; i < n; ++i)
    scanf("%lf %lf", &points[i].x, &points[i].y);

  for (i = 0; i < n; ++i)
    for (j = i + 1; j < n; ++j)
      for (k = j + 1; k < n; ++k)
        max_area = max(max_area, area(points[i], points[j], points[k]));

  printf("%.6lf\n", max_area);
  return 0;
}

