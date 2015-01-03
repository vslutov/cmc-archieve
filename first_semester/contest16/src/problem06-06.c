/*
    problem06-06 - list substraction
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

    Output elements of first list, which not contain in second list.
*/

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct _List
{
  intmax_t value;
  struct _List *next;
} *List;

List
new_list (void)
{
  List list = (List) malloc( sizeof(struct _List) );
  list->next = list;
  return list;
}

List
append (List list, intmax_t value)
{
  while (list->next != list)
    list = list->next;

  List new_one = new_list();

  list->value = value;
  list->next = new_one;

  return list;
}

bool
in_list (List list, intmax_t value)
{
  while (list->next != list)
    if (list->value == value)
      return true;
    else
      list = list->next;

  return false;
}

void
remove_list (List elem)
{
  List next = elem->next;

  if (next->next != next)
    elem->next = next->next;
  else
    elem->next = elem;
  elem->value = next->value;

  free(next);
}

void
delete_list (List list)
{
  while (list->next != list)
    {
      List next = list->next;
      free(list);
      list = next;
    }
  free(list);
}

int
main (void)
{
  assert(freopen("input.txt", "r", stdin) != NULL);
  assert(freopen("output.txt", "w", stdout) != NULL);

  List first = new_list(),
       end = first,
       second = new_list();

  intmax_t value;

  do
    {
      scanf("%jd", &value);
      if (value != -1)
        append(end, value);
    }
  while (value != -1);

  end = second;
  do
    {
      scanf("%jd", &value);
      if ((value != -1) && (!in_list(second, value)))
        append(end, value);
    }
  while (value != -1);

  List elem = first;
  while (elem->next != elem)
    {
      while ((elem->next != elem) && in_list(second, elem->value))
        remove_list(elem);

      if (elem->next != elem)
        {
          printf("%jd ", elem->value);
          remove_list(elem);
        }
    }

  printf("\n");

  remove_list(first);
  delete_list(second);

  fclose(stdin);
  fclose(stdout);

  return 0;
}

