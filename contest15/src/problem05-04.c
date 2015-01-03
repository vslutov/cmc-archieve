/*
    problem05-04 - less
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

    How many elements less than last one?
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

  unsigned last_element = vector->array[vector->size - 1];

  unsigned count = 0;
  for (size_t i = 0; i < vector->size; ++i)
    if (vector->array[i] < last_element)
      ++ count;

  printf("%u\n", count);

  delete_vector_unsigned(vector);

  return 0;
}

