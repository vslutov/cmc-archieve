/*
    queue - hand-made queue implementation
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

#define QUEUE_INIT_SIZE 1
#define QUEUE_FACTOR 2

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#define QUEUE(TYPE) \
\
typedef struct Queue_##TYPE_struct \
{ \
  TYPE *array; \
  size_t real_size; \
  size_t begin, end; \
} *Queue_##TYPE; \
\
Queue_##TYPE \
new_queue_##TYPE (void) \
{ \
  Queue_##TYPE queue = (Queue_##TYPE) \
                       malloc( sizeof(struct Queue_##TYPE_struct) ); \
  queue->real_size = QUEUE_INIT_SIZE; \
  queue->begin = 0; \
  queue->end = 0; \
  queue->array = (TYPE *) malloc(sizeof(TYPE) * queue->real_size); \
  assert(queue->array != NULL); \
  return queue; \
} \
\
void \
delete_queue_##TYPE (Queue_##TYPE queue) \
{ \
  free(queue->array); \
  free(queue); \
} \
\
void \
push_back_queue_##TYPE (Queue_##TYPE queue, TYPE element) \
{ \
  queue->array[queue->end] = element; \
  ++ queue->end; \
  if (queue->end == queue->real_size) \
    { \
      queue->real_size *= QUEUE_FACTOR; \
      queue->array = (TYPE *) realloc(queue->array, \
                                      sizeof(TYPE) * queue->real_size); \
    } \
} \
\
TYPE \
pop_front_queue_##TYPE (Queue_##TYPE queue) \
{ \
  TYPE result = queue->array[queue->begin]; \
  ++ queue->begin; \
  if (queue->begin == queue->real_size / 2) \
    { \
      for (size_t i = queue->begin; i < queue->end; ++i) \
        queue->array[i - queue->begin] = queue->array[i]; \
      queue->begin = 0; \
      queue->end = queue->end - queue->real_size / 2; \
    } \
  return result; \
} \
\
size_t \
length_queue_##TYPE (Queue_##TYPE queue) \
{ \
  return queue->end - queue->begin; \
} \
\
bool \
is_empty_queue_##TYPE (Queue_##TYPE queue) \
{ \
  return length_queue_##TYPE(queue) == 0; \
}

