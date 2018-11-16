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

#include <stdio.h>
#include <math.h>

#include "apmath.h"

double_t
_f_one (double_t x)
{
  return 3.0 * (0.5 / (x + 1.0) + 1);
}

double_t
_f_two (double_t x)
{
  return 2.5 * x - 9.5;
}

double_t
_f_three (double_t x)
{
  return 5.0 / x;
}

int
main (void)
{
  Function f_one = wrap(_f_one);
  Function f_two = wrap(_f_two);
  Function f_three = wrap(_f_three);

  Function sub_one_two = substract(f_one, f_two);
  Function sub_two_three = substract(f_two, f_three);
  Function sub_three_one = substract(f_three, f_one);

  const double_t eps = 1e-3;

  const double_t max_value = 10.0;
  const double_t lower = 0.5;
  const double_t upper = 6.0;

  const double_t x_one_two = solutionCombinedMethod(sub_one_two, lower, upper,
                              eps * 0.1 / max_value);
  const double_t x_two_three = solutionCombinedMethod(sub_two_three, lower,
                                upper, eps * 0.1 / max_value);
  const double_t x_three_one = solutionCombinedMethod(sub_three_one, lower,
                                upper, eps * 0.1 / max_value);

  printf("Intersection points:\n");
  printf("  %.4lf\n", (double)x_one_two);
  printf("  %.4lf\n", (double)x_two_three);
  printf("  %.4lf\n", (double)x_three_one);

  const double_t area = fabs(
           + integralSimpsonsRule(f_one, x_three_one, x_one_two, eps * 0.1)
           + integralSimpsonsRule(f_two, x_one_two, x_two_three, eps * 0.1)
           + integralSimpsonsRule(f_three, x_two_three, x_three_one, eps * 0.1)
         );

  printf("Calculated area: %.4lf (accuracy = 1e-3)\n", (double)area);

  return 0;
}
