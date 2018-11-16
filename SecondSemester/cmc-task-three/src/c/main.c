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

#include <stdio.h>

int
main (void)
{
  #if TEST_OUTPUT == TEST_IMG
    stdout = freopen("img/data.txt", "w", stdout);
  #endif

  Sort_algorithm bubble_sort = new_bubble_sort();

  #if TEST_OUTPUT == TEST_CONSOLE
    printf("Bubble sort:\n\n");
  #endif
  test_algorithm(bubble_sort, 10000);

  delete_bubble_sort(bubble_sort);

  Sort_algorithm heap_sort = new_heap_sort();

  #if TEST_OUTPUT == TEST_CONSOLE
    printf("Heap sort:\n\n");
  #endif
  test_algorithm(heap_sort, 1000000);

  delete_heap_sort(heap_sort);

  return 0;
}
