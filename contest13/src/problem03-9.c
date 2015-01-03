/*
    problem03-9 - triangle bisectors
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

    Output triangle bisectors bases.
*/

#include <math.h>
#include <stdio.h>

struct Vector
{
  double x, y;
};

struct Vector
Vscan (void)
{
  struct Vector A;
  scanf("%lf %lf", &A.x, &A.y);
  return A;
}

void
Vprint (struct Vector A)
{
  printf("%.5lf %.5lf\n", A.x, A.y);
}

struct Vector
sub (struct Vector A, struct Vector B)
{
  B.x -= A.x;
  B.y -= A.y;
  return B;
}

struct Vector
add (struct Vector A, struct Vector B)
{
  B.x += A.x;
  B.y += A.y;
  return B;
}

double
inner_product (struct Vector a, struct Vector b)
{
  return a.x * b.x + a.y * b.y;
}

struct Vector
mul (struct Vector a, double k)
{
  a.x = a.x * k;
  a.y = a.y * k;
  return a;
}

double
Vabs (struct Vector a)
{
  return sqrt( inner_product(a, a) );
}

int
main (void)
{
  size_t i;
  struct Vector points[3], sides[3];

  for (i = 0; i < 3; ++i)
    points[i] = Vscan();
  for (i = 0; i < 3; ++i)
    sides[i] = sub(points[i], points[(i+1) % 3]);

  for (i = 0; i < 3; ++i)
    {
      // For example we find Cb
      double a = Vabs(sides[(i+2) % 3]); // a = |AC|
      double b = Vabs(sides[(i+1) % 3]); // b = |BC|
      struct Vector B = add(points[i], mul(sides[i], a / (a + b)));
      // Cb = A + AB * |AC| / (|AC| + |BC|)
      Vprint(B);
    }

  return 0;
}

