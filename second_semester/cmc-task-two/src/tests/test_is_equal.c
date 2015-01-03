/*
    LICENSE - GNU GPL v3

    cmc-task-two - check two polynomials equivalence
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

#include "polynomial.h"

#include <assert.h>
#include <stdio.h>

#define TEST_EQUAL(str_polynomial_one, str_polynomial_two, answer) \
{ \
  Polynomial polynomial_one = stringToPolynomial(str_polynomial_one " "); \
  Polynomial polynomial_two = stringToPolynomial(str_polynomial_two " "); \
  bool result = isEqualPolynomial(polynomial_one, polynomial_two); \
  assert(result == answer); \
  printf("<tr>"); \
  printf("<td>%s</td>", str_polynomial_one); \
  printf("<td>%s</td>", str_polynomial_two); \
  if (result) \
    printf("<td>YES</td>"); \
  else \
    printf("<td>NO</td>"); \
  printf("</tr>"); \
}

int
main (int argc, char **argv)
{
  printf("<table>");
  printf("<caption>isEqual tests</caption>");
  printf("<thead>");
  printf("<tr>");
  printf("<td>First polynomial</td>");
  printf("<td>Second polynomial</td>");
  printf("<td>Is equal?</td>");
  printf("</tr>");
  printf("</thead>");
  printf("<tbody>");
  TEST_EQUAL("X-5", "-5+X", true);
  TEST_EQUAL("X", "X", true);
  TEST_EQUAL("-X", "-X", true);
  TEST_EQUAL("-X", "X", false);
  TEST_EQUAL("X-3X^2", "X^48", false);
  TEST_EQUAL("42", "-5+X^11", false);
  TEST_EQUAL("1+X^11", "5+X^11", false);
  TEST_EQUAL("5+3X^12+X^4", "3X^12+5+X^4", true);
  TEST_EQUAL("X-10+X^3-X^4", "-10+X^3-X^4+X", true);
  printf("</tbody>");
  printf("</table>\n");

  return 0;
}

