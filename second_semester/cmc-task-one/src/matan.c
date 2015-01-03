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

#include "apmath.h"
#include "apmath-dev.h"

#include "helpers.h"

#include <stdlib.h>

// Mathematical analysis

double_t
derivative (Function f, double_t x, double_t eps)
{
  double_t dx = 1;
  double_t dp, dn;

  double_t new_eps = eps * 0.3;
  while (1)
  {
    dx = dx * 0.1;

    double_t a = eps * dx * 0.08; // eps * dx / 12
    double_t f0 = calc(f, x, a);
    dp = (calc(f, x + dx, a) - f0) / dx;
    dn = (calc(f, x - dx, a) - f0) / -dx;
    if (helpers_eq(dp, dn, new_eps))
      break;
  }

  return (dp + dn) * 0.5;
}

double_t
_derivative_calc(Function this, double_t x, double_t eps)
{
  Function f = *(Function *)this->data;
  return derivative(f, x, eps);
}

Function
derivate (Function f)
{
  Function this = (Function)malloc( sizeof(struct _FunctionStruct) );
  this->calc = _derivative_calc;
  this->data = malloc( sizeof(struct _FunctionStruct) );
  *(Function *)this->data = f;
  return this;
}

