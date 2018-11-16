/*
    problem06-17 - big search tree (copy of problem06-04)
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

    Implement functions:

      add(tree, key, value)
      delete(tree, key)
      search(tree, key) => value
      finito(tree)
*/

#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "treap.template.h"

typedef intmax_t KEY_TYPE, VALUE_TYPE;

typedef struct
{
  KEY_TYPE key;
  VALUE_TYPE value;
} Pair;

bool
less (Pair a, Pair b)
{
  return a.key < b.key;
}

TREAP(Pair, less)

typedef Treap_Pair Tree;

void
add (Tree tree, KEY_TYPE key, VALUE_TYPE value)
{
  Pair pair = {.key = key, .value = value};
  if (in_tree_treap_Pair(tree, pair))
    remove_treap_Pair(tree, pair);

  insert_treap_Pair(tree, pair);
}

void
delete (Tree tree, KEY_TYPE key)
{
  Pair pair = {.key = key};
  if (in_tree_treap_Pair(tree, pair))
    remove_treap_Pair(tree, pair);
}

void
search (Tree tree, KEY_TYPE key)
{
  Pair pair = {.key = key};
  if (in_tree_treap_Pair(tree, pair))
  {
    _Treap_pnode_Pair v = *tree;
    while (v->key.key != key)
      if (less(pair, v->key))
        v = v->l;
      else // less(v-key, pair)
        v = v->r;

    printf("%jd %jd\n", key, v->key.value);
  }
}

void
finito (Tree tree)
{
  delete_treap_Pair(tree);
}

int
main (void)
{
  Tree tree = new_treap_Pair();

  char command[2];
  bool playback = true;

  while (playback)
  {
    intmax_t key, value;
    scanf("%s", command);
    if (strcmp(command, "A") == 0)
      {
        scanf("%jd %jd", &key, &value);
        add(tree, key, value);
      }
    else if (strcmp(command, "D") == 0)
      {
        scanf("%jd", &key);
        delete(tree, key);
      }
    else if (strcmp(command, "S") == 0)
      {
        scanf("%jd", &key);
        search(tree, key);
      }
    else
      {
        finito(tree);
        playback = false;
      }
  }

  return 0;
}

