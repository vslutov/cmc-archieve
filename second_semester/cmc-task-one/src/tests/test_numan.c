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

// Numerical analysis test

#include "apmath.h"
#include "apmath-dev.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <math.h>

const double_t EPS = 1e-6;

#define ASSERT_EQ(value, expression, eps)\
{ \
  resetCounter(); \
  double_t result = expression; \
  assert( eq(value, result, eps) ); \
  printf("<tr>"); \
  printf("<td>%s</td>", #expression); \
  printf("<td>%.7f</td>", (double)result); \
  printf("<td>%.7f</td>", (double)value); \
  printf("<td>%.7f</td>", (double)eps); \
  printf("<td>%d</td>", (int)getCounter()); \
  printf("</tr>"); \
}

double_t
_sqr (double_t x)
{
  return 3 * x * x;
}

bool
eq (double_t x, double_t y, double_t eps)
{
  return fabs(x - y) <= eps;
}

const double_t ROOT = 5;

double_t
_hyperbola_one (double_t x)
{
  return 10.0 / (x + 5.0) - 1.0;
}

double_t
_hyperbola_two (double_t x)
{
  return -_hyperbola_one(x);
}

double_t
_hyperbola_three (double_t x)
{
  return _hyperbola_one(-x);
}

double_t
_hyperbola_four (double_t x)
{
  return -_hyperbola_one(-x);
}

double_t
_sin (double_t x)
{
  return sin(x);
}

double_t
_magic (double_t x)
{
  return pow(x, x) * (log(x) + 1);
}

int
main (void)
{
  Function Hyperbola_one = wrap(_hyperbola_one);
  Function Hyperbola_two = wrap(_hyperbola_two);
  Function Hyperbola_three = wrap(_hyperbola_three);
  Function Hyperbola_four = wrap(_hyperbola_four);

  printf("<table>");
  printf("<caption>Solution tests</caption>");
  printf("<thead>");
  printf("<tr>");
  printf("<td>Expression</td>");
  printf("<td>Value</td>");
  printf("<td>Expected</td>");
  printf("<td>EPS</td>");
  printf("<td>Steps</td>");
  printf("</tr>");
  printf("</thead>");

  // BisectionMethod

  ASSERT_EQ(ROOT, solutionBisectionMethod(Hyperbola_one, 0, 20, EPS), EPS);
  ASSERT_EQ(ROOT, solutionBisectionMethod(Hyperbola_two, 0, 20, EPS), EPS);
  ASSERT_EQ(-ROOT, solutionBisectionMethod(Hyperbola_three, -20, 0, EPS), EPS);
  ASSERT_EQ(-ROOT, solutionBisectionMethod(Hyperbola_four, -20, 0, EPS), EPS);

  // ChordMethod

  ASSERT_EQ(ROOT, solutionChordMethod(Hyperbola_one, 0, 20, EPS), EPS);
  ASSERT_EQ(ROOT, solutionChordMethod(Hyperbola_two, 0, 20, EPS), EPS);
  ASSERT_EQ(-ROOT, solutionChordMethod(Hyperbola_three, -20, 0, EPS), EPS);
  ASSERT_EQ(-ROOT, solutionChordMethod(Hyperbola_four, -20, 0, EPS), EPS);

  // NewtonsMetod

  ASSERT_EQ(ROOT, solutionNewtonsMethod(Hyperbola_one, 0, 20, EPS), EPS);
  ASSERT_EQ(ROOT, solutionNewtonsMethod(Hyperbola_two, 0, 20, EPS), EPS);
  ASSERT_EQ(-ROOT, solutionNewtonsMethod(Hyperbola_three, -20, 0, EPS), EPS);
  ASSERT_EQ(-ROOT, solutionNewtonsMethod(Hyperbola_four, -20, 0, EPS), EPS);

  // CombinedMethod

  ASSERT_EQ(ROOT, solutionCombinedMethod(Hyperbola_one, 0, 20, EPS), EPS);
  ASSERT_EQ(ROOT, solutionCombinedMethod(Hyperbola_two, 0, 20, EPS), EPS);
  ASSERT_EQ(-ROOT, solutionCombinedMethod(Hyperbola_three, -20, 0, EPS), EPS);
  ASSERT_EQ(-ROOT, solutionCombinedMethod(Hyperbola_four, -20, 0, EPS), EPS);

  ASSERT_EQ(ROOT, solutionCombinedMethod(Hyperbola_one, 0, 20, EPS * 10),
            EPS * 10);
  ASSERT_EQ(ROOT, solutionCombinedMethod(Hyperbola_one, 0, 20, EPS * 100),
            EPS * 100);
  ASSERT_EQ(ROOT, solutionCombinedMethod(Hyperbola_one, 0, 20, EPS * 1000),
            EPS * 1000);
  ASSERT_EQ(ROOT, solutionCombinedMethod(Hyperbola_one, 0, 20, EPS * 10000),
            EPS * 10000);
  ASSERT_EQ(ROOT, solutionCombinedMethod(Hyperbola_one, 0, 20, EPS * 100000),
            EPS * 100000);

  printf("</table>\n");

  printf("<table>");
  printf("<caption>Integral tests</caption>");
  printf("<thead>");
  printf("<tr>");
  printf("<td>Expression</td>");
  printf("<td>Value</td>");
  printf("<td>Expected</td>");
  printf("<td>EPS</td>");
  printf("<td>Steps</td>");
  printf("</tr>");
  printf("</thead>");

  Function sqr = wrap(_sqr);
  Function wsin = wrap(_sin);
  Function dx_x_power_x = wrap(_magic);

  ASSERT_EQ(125.0, integralRectangleMethod(sqr, 0, 5.0, EPS), EPS);
  ASSERT_EQ(1.0 , integralRectangleMethod(wsin, 0, M_PI * 0.5, EPS), EPS);
  ASSERT_EQ(1.0 , integralRectangleMethod(wsin, 0, M_PI * 1.5, EPS), EPS);
  ASSERT_EQ(3124.0 , integralRectangleMethod(dx_x_power_x, 1, 5, EPS), EPS);

  ASSERT_EQ(125.0, integralTrapezoidalMethod(sqr, 0, 5.0, EPS), EPS);
  ASSERT_EQ(1.0 , integralTrapezoidalMethod(wsin, 0, M_PI * 0.5, EPS), EPS);
  ASSERT_EQ(1.0 , integralTrapezoidalMethod(wsin, 0, M_PI * 1.5, EPS), EPS);
  ASSERT_EQ(3124.0 , integralTrapezoidalMethod(dx_x_power_x, 1, 5, EPS), EPS);

  ASSERT_EQ(125.0, integralSimpsonsRule(sqr, 0, 5.0, EPS), EPS);
  ASSERT_EQ(1.0 , integralSimpsonsRule(wsin, 0, M_PI * 0.5, EPS), EPS);
  ASSERT_EQ(1.0 , integralSimpsonsRule(wsin, 0, M_PI * 1.5, EPS), EPS);
  ASSERT_EQ(3124.0 , integralSimpsonsRule(dx_x_power_x, 1, 5, EPS), EPS);

  ASSERT_EQ(3124.0 , integralSimpsonsRule(dx_x_power_x, 1, 5, EPS * 10),
            EPS * 10);
  ASSERT_EQ(3124.0 , integralSimpsonsRule(dx_x_power_x, 1, 5, EPS * 100),
            EPS * 100);
  ASSERT_EQ(3124.0 , integralSimpsonsRule(dx_x_power_x, 1, 5, EPS * 1000),
            EPS * 1000);
  ASSERT_EQ(3124.0 , integralSimpsonsRule(dx_x_power_x, 1, 5, EPS * 10000),
            EPS * 10000);
  ASSERT_EQ(3124.0 , integralSimpsonsRule(dx_x_power_x, 1, 5, EPS * 100000),
            EPS * 100000);

  printf("</table>\n");
  return 0;
}

