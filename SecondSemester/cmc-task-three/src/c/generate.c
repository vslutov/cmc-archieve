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

#include <stdlib.h>
#include <time.h>

void
generate_sorted_array (Sort_array_pointer array, size_t length)
{
  for (size_t i = 0; i < length; ++ i)
  {
    #if SORT_KIND == ASCENDING
      array[i] = (Sort_type) i;
    #else
      array[i] = - (Sort_type) i;
    #endif
  }
}

void
generate_desorted_array (Sort_array_pointer array, size_t length)
{
  for (size_t i = 0; i < length; ++ i)
  {
    #if SORT_KIND == ASCENDING
      array[i] = - (Sort_type) i;
    #else
      array[i] = (Sort_type) i;
    #endif
  }
}

void
generate_random_array (Sort_array_pointer array, size_t length)
{
  srand( time(NULL) );
  for (size_t i = 0; i < length; ++ i)
    array[i] = (((Sort_type) rand()) * ((Sort_type) rand()) *
		            ((Sort_type) rand()) * ((Sort_type) rand())) /
							 (((Sort_type) RAND_MAX) * ((Sort_type) RAND_MAX) *
							 	((Sort_type) RAND_MAX));
}

