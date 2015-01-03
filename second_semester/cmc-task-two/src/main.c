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

#include <stdio.h>

int
main (int argc, char **argv)
{
  Polynomial one = fileToPolynomial(stdin);
  Polynomial two = fileToPolynomial(stdin);
  if ( isEqualPolynomial(one, two) )
    printf("YES\n");
  else
    printf("NO\n");
  return 0;
}

