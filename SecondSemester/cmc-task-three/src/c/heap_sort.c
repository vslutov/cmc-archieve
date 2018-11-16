/*
 *  cmc-task-three - comparation of some sort method
 *  Copyright (C) 2014 V. S. Lutov
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "global.h"

void
sift (Sort_array_pointer array, size_t i, size_t length)
{
  size_t left = 2 * i + 1;
  size_t right = left + 1;
  size_t max_child;
  while (left < length)
  {
    if (right < length && sort_pre(array[left], array[right]))
      max_child = right;
    else
      max_child = left;

    if ( sort_pre(array[i], array[max_child]) )
      sort_swap(&array[i], &array[max_child]);
    else
      break;

    i = max_child;
    left = 2 * i + 1;
    right = left + 1;
  }
}

void
_heap_sort (Sort_algorithm this)
{
  for (size_t i = this->length / 2; i != (size_t)(-1); -- i)
    sift(this->array, i, this->length);

  for (size_t j = this->length - 1; j > 0; -- j)
    {
      sort_swap(&this->array[0], &this->array[j]);
      sift(this->array, 0, j);
    }
}

Sort_algorithm
new_heap_sort (void)
{
  Sort_algorithm heap_sort = malloc( sizeof(struct _Sort_algorithm_struct) );

  heap_sort->init = NULL;
  heap_sort->sort = _heap_sort;
  heap_sort->deinit = NULL;

  return heap_sort;
}

void
delete_heap_sort (Sort_algorithm algorithm)
{
  free(algorithm);
}

