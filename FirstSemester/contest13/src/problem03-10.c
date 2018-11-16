/*
    problem03-10 - calc
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

    Return value of expression in polish notation

    Polish Notation - P

    1) N in P
    2) If (A in P) and (B in P) : (* A B) in P
    3) If (A in P) and (B in P) : (/ A B) in P
*/

#include <stdio.h>
#include <inttypes.h>

intmax_t
get_value (void)
{
  intmax_t a;
  if (scanf("%ju", &a) == 1)
    return a;
  else if (getchar() == '*')
    return get_value() * get_value();
  else // if (getchar() == '/'
    return get_value() / get_value();
}

int
main (void)
{
  printf("%jd\n", get_value());
  return 0;
}

