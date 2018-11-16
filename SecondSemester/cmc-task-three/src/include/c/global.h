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

#ifndef GLOBAL_H
#define GLOBAL_H

#define ASCENDING 1
#define DESCENDING 2

#ifndef SORT_KIND
  #error UNDEFINED SORT KIND
#elif SORT_KIND != ASCENDING && SORT_KIND != DESCENDING
  #error INVALID SORT KIND
#endif

#define TEST_CONSOLE 1
#define TEST_IMG 2

#ifndef TEST_OUTPUT
  #define TEST_OUTPUT TEST_CONSOLE
#elif (TEST_OUTPUT != TEST_CONSOLE) && (TEST_OUTPUT != TEST_IMG)
  #define TEST_OUTPUT TEST_CONSOLE
#endif

#include <stdbool.h>
#include <stdlib.h>

typedef double Sort_type;
typedef Sort_type *Sort_array_pointer;

bool
sort_cmp_less (Sort_type, Sort_type);

void
sort_swap (Sort_type *, Sort_type *);

unsigned long long int
test_rdtsc (void);

#if SORT_KIND == ASCENDING
  #define sort_pre(X, Y) sort_cmp_less(X, Y)
#else
  #define sort_pre(X, Y) !sort_cmp_less(X, Y)
#endif

void
generate_sorted_array (Sort_array_pointer array, size_t length);

void
generate_desorted_array (Sort_array_pointer array, size_t length);

void
generate_random_array (Sort_array_pointer array, size_t length);


typedef struct _Sort_algorithm_struct *Sort_algorithm;

struct _Sort_algorithm_struct
{
  Sort_array_pointer array;
  size_t length;

  char *data;

  void (*init)(Sort_algorithm sort_algorithm);
  void (*sort)(Sort_algorithm algorithm);
  void (*deinit)(Sort_algorithm algorithm);
};

extern size_t test_cmp_counter;
extern size_t test_swap_counter;

void
test_algorithm (Sort_algorithm algorithm, size_t max_length);

Sort_algorithm
new_bubble_sort (void);

void
delete_bubble_sort (Sort_algorithm algorithm);

Sort_algorithm
new_heap_sort (void);

void
delete_heap_sort (Sort_algorithm algorithm);

#endif /* GLOBAL_H */
