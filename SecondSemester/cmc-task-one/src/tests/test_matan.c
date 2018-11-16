/*
    LICENSE - GNU GPL v3

    cmc-task-one - equation solving and definite integrals
    Copyright (C) 2014  V. S. Lutov

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

// Mathematical analysis test

#include "apmath.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include <math.h>

const double_t EPS = 1e-6;

bool
eq (double_t x, double_t y, double_t eps)
{
  return fabs(x - y) <= eps;
}

double_t
_sin (double_t x)
{
  return sin(x);
}

int
main (void)
{
  Function wsin = wrap(_sin);
  Function wcos = derivate(wsin);

  assert( eq(0.0, derivative(wsin, M_PI / 2.0, EPS), EPS) );
  assert( eq(0.5, calc(wcos, M_PI / 3.0, EPS), EPS) );
  return 0;
}

