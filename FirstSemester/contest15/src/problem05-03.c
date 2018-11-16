/*
    problem05-03 - sequence suffle
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

    Output first all odd-placed members of sequence.
    Then even.
*/

#include "vector.template.h"

#include <stdio.h>

VECTOR(unsigned)

int
main (void)
{
  Vector_unsigned vector = new_vector_unsigned();

  unsigned element;
  do
    {
      scanf("%u", &element);
      if (element != 0)
        push_back_vector_unsigned(vector, element);
    }
  while (element != 0);

  for (size_t i = 0; i < vector->size; i += 2)
    printf("%u ", vector->array[i]);
  for (size_t i = 1; i < vector->size; i += 2)
    printf("%u ", vector->array[i]);
  printf("\n");

  delete_vector_unsigned(vector);

  return 0;
}

