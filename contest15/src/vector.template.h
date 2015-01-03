/*
    vector - hand-made vector implementation
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

#include <assert.h>
#include <stdlib.h>

#define VECTOR_INIT_SIZE 1
#define VECTOR_FACTOR 2

#define VECTOR(TYPE) \
\
typedef struct Vector_##TYPE_struct \
{ \
  TYPE *array; \
  size_t real_size; \
  size_t size; \
} *Vector_##TYPE; \
\
Vector_##TYPE \
new_vector_##TYPE (void) \
{ \
  Vector_##TYPE vector = (Vector_##TYPE) \
                         malloc( sizeof(struct Vector_##TYPE_struct) ); \
  vector->real_size = VECTOR_INIT_SIZE; \
  vector->size = 0; \
  vector->array = (TYPE *) malloc(sizeof(TYPE) * vector->real_size); \
  assert(vector->array != NULL); \
  return vector; \
} \
\
void \
delete_vector_##TYPE (Vector_##TYPE vector) \
{ \
  free(vector->array); \
  free(vector); \
} \
\
void \
push_back_vector_##TYPE (Vector_##TYPE vector, TYPE element) \
{ \
  vector->array[vector->size] = element; \
  ++ vector->size; \
  if (vector->size == vector->real_size) \
    { \
      vector->real_size *= VECTOR_FACTOR; \
      vector->array = (TYPE *) realloc(vector->array, \
                                       sizeof(TYPE) * vector->real_size); \
    } \
} \
\
TYPE \
pop_back_vector_##TYPE (Vector_##TYPE vector) \
{ \
  return vector->array[-- vector->size]; \
}

