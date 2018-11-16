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

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#if TEST_OUTPUT == TEST_CONSOLE
  const size_t TEST_MIN_LENGTH = 10;
  const size_t TEST_MULTIPLIER = 2;
  const size_t TEST_SUMMAND = 0;

  const size_t TEST_MIN_ITERATION = 10;
  const size_t TEST_MAX_ITERATION = 300;
#elif TEST_OUTPUT == TEST_IMG
  const size_t TEST_MIN_LENGTH = 10;
  const size_t TEST_MULTIPLIER = 1;
  const size_t TEST_SUMMAND = 2;

  const size_t TEST_MIN_ITERATION = 10;
  const size_t TEST_MAX_ITERATION = 300;
#endif

const double TEST_EPS = 0.1;
const double DOUBLE_EPS = 0.1;

const size_t VECTOR_START_SIZE = 10;
const size_t VECTOR_MULTIPLER = 2;

size_t test_number;
size_t test_length;

clock_t test_time;
unsigned long long int test_tacts;

size_t test_cmp_counter;
size_t test_swap_counter;

typedef struct
{
  double time, tacts, cmps, swaps;
} Test_info;

double test_average_time, test_average_tacts,
       test_average_cmps, test_average_swaps;

double test_accuracy_time, test_accuracy_tacts,
       test_accuracy_cmps, test_accuracy_swaps;

Test_info *test_array;
size_t test_array_length;

size_t test_iteration;

void
start_test (void)
{
  test_time = clock();
  test_cmp_counter = 0;
  test_swap_counter = 0;
  test_tacts = test_rdtsc();
}

void
stop_test (void)
{
  test_tacts = test_rdtsc() - test_tacts;
  test_time = clock() - test_time;

  test_array[test_iteration].time = test_time;
  test_array[test_iteration].tacts = test_tacts;
  test_array[test_iteration].cmps = test_cmp_counter;
  test_array[test_iteration].swaps = test_swap_counter;

  ++ test_iteration;
  if (test_iteration == test_array_length)
    {
      test_array_length = VECTOR_MULTIPLER * test_array_length;
      test_array = realloc(test_array, test_array_length *
                           sizeof(Test_info));
    }
}

#define AVERAGE(NAME) \
{ \
  double sum_##NAME = 0; \
  for (size_t i = 0; i < test_iteration; ++ i) \
    sum_##NAME += test_array[i].NAME; \
  test_average_##NAME = sum_##NAME / test_iteration; \
  sum_##NAME = 0; \
  for (size_t i = 0; i < test_iteration; ++ i) \
    sum_##NAME += (test_array[i].NAME - test_average_##NAME) * \
                  (test_array[i].NAME - test_average_##NAME); \
  test_accuracy_##NAME = sqrt(sum_##NAME / test_iteration); \
}

bool
sure (void)
{
  if (test_iteration < TEST_MIN_ITERATION)
    return false;

  AVERAGE(time);
  AVERAGE(tacts);
  AVERAGE(cmps);
  AVERAGE(swaps);

  return (test_iteration == TEST_MAX_ITERATION) ||
       ( (test_accuracy_tacts <= TEST_EPS * test_average_tacts + DOUBLE_EPS) &&
         (test_accuracy_cmps <= TEST_EPS * test_average_cmps + DOUBLE_EPS) &&
         (test_accuracy_swaps <= TEST_EPS * test_average_swaps + DOUBLE_EPS) );
}

#undef AVERAGE

void
print_group_header (size_t group_number, char *group_name)
{
  #if TEST_OUTPUT == TEST_CONSOLE
    printf("Test group %zu: %s\n", group_number, group_name);
    printf("|Length      |Comparations|Swaps       |Tacts       |Time (ms)   |\n");
    printf("|-----------:|-----------:|-----------:|-----------:|-----------:|\n");
  #elif TEST_OUTPUT == TEST_IMG
  #endif
}

void
print_test_info (void)
{
  #if TEST_OUTPUT == TEST_CONSOLE
    printf("|%12zu|%12.0f|%12.0f|%12.0f|%12.1f|\n", test_length, test_average_cmps, test_average_swaps, test_average_tacts, 1000 * test_average_time / CLOCKS_PER_SEC);
  #elif TEST_OUTPUT == TEST_IMG
    printf("%zu %.0f %.0f %.0f %.1f ", test_length, test_average_cmps, test_average_swaps, test_average_tacts, 1000 * test_average_time / CLOCKS_PER_SEC);
  #endif
}

void
print_group_footer (void)
{
  #if TEST_OUTPUT == TEST_CONSOLE
    printf("\n\n");
  #elif TEST_OUTPUT == TEST_IMG
    printf("\n\n");
  #endif
}

void
test_algorithm (Sort_algorithm algorithm, size_t max_length)
{
  #if TEST_OUTPUT == TEST_IMG
    max_length = 100;
  #endif
  Sort_array_pointer array = malloc (max_length * sizeof(Sort_type));
  test_array = malloc(sizeof(Test_info) * VECTOR_START_SIZE);
  test_array_length = VECTOR_START_SIZE;

  for (size_t i = 1; i <= 3; ++ i)
    {
      void (*generate_array)(Sort_array_pointer, size_t);

      switch (i)
        {
          case 1:
          print_group_header(i, "Sorted array");
          generate_array = generate_sorted_array;
          break;
          case 2:
          print_group_header(i, "Reverse sorted array");
          generate_array = generate_desorted_array;
          break;
          default:
          print_group_header(i, "Random array");
          generate_array = generate_random_array;
        }

      test_number = 0;

      for (test_length = TEST_MIN_LENGTH;
           test_length <= max_length;
           test_length = test_length * TEST_MULTIPLIER + TEST_SUMMAND)
        {
          ++ test_number;

          test_iteration = 0;

          do
            {
              generate_array (array, test_length);

              algorithm->array = array;
              algorithm->length = test_length;

              if (algorithm->init != NULL)
                algorithm->init(algorithm);

              start_test();

              algorithm->sort(algorithm);

              stop_test();

              if (algorithm->deinit != NULL)
                algorithm->deinit(algorithm);
            }
          while (!sure());

          print_test_info();
        }

      print_group_footer();
    }

  free(array);
}

