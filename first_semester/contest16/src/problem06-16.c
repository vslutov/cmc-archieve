/*
    problem06-16 - list sort
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

    Sort list.
*/

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef intmax_t TYPE_VALUE;

typedef struct _ListNodeStruct
{
  TYPE_VALUE value;
  struct _ListNodeStruct *next;
} *ListNodePointer;

typedef struct _ListStruct
{
  ListNodePointer first, last;
  size_t size;
} *List;

List
new_list (void)
{
  List list = (List) malloc( sizeof(struct _ListStruct) );
  list->first = NULL;
  list->last = NULL;
  list->size = 0;
  return list;
}

void
delete_list (List list)
{
  ListNodePointer next;
  for (ListNodePointer node = list->first; node != NULL; node = next)
    {
      next = node->next;
      free(node);
    }
  free(list);
}

void
list_copy (List a, const List b)
{
  memcpy(a, b, sizeof(struct _ListStruct));
}

void
push_back (List list, TYPE_VALUE value)
{
  ListNodePointer node = (ListNodePointer)
                         malloc( sizeof(struct _ListNodeStruct) );
  node->value = value;
  node->next = NULL;

  if (list->last != NULL)
    list->last->next = node;
  list->last = node;

  if (list->first == NULL)
    list->first = node;

  ++ list->size;
}

void
push_front (List list, TYPE_VALUE value)
{
  ListNodePointer node = (ListNodePointer)
                         malloc( sizeof(struct _ListNodeStruct) );
  node->value = value;
  node->next = list->first;

  if (list->first == NULL)
    list->first = node;

  if (list->last == NULL)
    list->last = node;

  ++ list->last;
}

TYPE_VALUE
pop_front (List list)
{
  ListNodePointer node = list->first;

  list->first = node->next;
  if (node->next == NULL)
    list->last = NULL;

  TYPE_VALUE result = node->value;
  free(node);

  -- list->size;

  return result;
}

bool
is_empty (List list)
{
  return list->size == 0;
}

List
merge (List a, List b)
{
  List result = new_list();

  while (!is_empty(a) && !is_empty(b))
    if (a->first->value < b->first->value)
      push_back(result, pop_front(a));
    else
      push_back(result, pop_front(b));

  while (!is_empty(a))
    push_back(result, pop_front(a));

  while (!is_empty(b))
    push_back(result, pop_front(b));

  delete_list(a);
  delete_list(b);

  return result;
}

void
merge_sort (List list)
{
  size_t size = list->size;

  if (size <= 1)
    return;

  List first_part = new_list(),
       second_part = new_list();

  for (size_t i = 0; i < size / 2; ++ i)
    push_back(first_part, pop_front(list));

  list_copy(second_part, list);

  merge_sort(first_part);
  merge_sort(second_part);

  List sorted = merge(first_part, second_part);

  list_copy(list, sorted);

  free(sorted);
}

void
print_list (List list)
{
  for (ListNodePointer node = list->first; node != NULL; node = node->next)
    printf("%jd ", node->value);
  printf("\n");
}

int
main (void)
{
  freopen("input.txt", "r", stdin);
  freopen("output.txt", "w", stdout);

  intmax_t value;
  List list = new_list();

  while (scanf("%jd", &value) != EOF)
    push_back(list, value);

  merge_sort(list);

  print_list(list);

  delete_list(list);

  fclose(stdin);
  fclose(stdout);

  return 0;
}

