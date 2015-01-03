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
_bubble_sort (Sort_algorithm this)
{
  for (size_t i = 0; i < this->length; ++ i)
    {
      bool flag = false;
      for (size_t j = 0; j < this->length - i - 1; ++ j)
        if (sort_pre(this->array[j + 1], this->array[j]))
          {
            sort_swap(&this->array[j], &this->array[j + 1]);
            flag = true;
          }

      if (!flag)
        break;
    }
}

Sort_algorithm
new_bubble_sort (void)
{
  Sort_algorithm bubble_sort = malloc( sizeof(struct _Sort_algorithm_struct) );

  bubble_sort->init = NULL;
  bubble_sort->sort = _bubble_sort;
  bubble_sort->deinit = NULL;

  return bubble_sort;
}

void
delete_bubble_sort (Sort_algorithm algorithm)
{
  free(algorithm);
}

