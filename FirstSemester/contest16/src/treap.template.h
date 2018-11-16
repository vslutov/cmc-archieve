/*
    treap - hand-made treap implementation (from e-maxx.ru)
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

#include <stdbool.h>
#include <stdlib.h>

#define TREAP(TYPE, LESS) \
typedef struct _Treap_struct_##TYPE { \
  TYPE key; \
  int priority; \
  struct _Treap_struct_##TYPE *l, *r; \
} *_Treap_pnode_##TYPE, **Treap_##TYPE; \
\
bool \
_Treap_##TYPE_equal (TYPE x, TYPE y) \
{ \
  return !LESS(x, y) && !LESS(y, x); \
} \
\
Treap_##TYPE \
new_treap_##TYPE (void) \
{ \
  Treap_##TYPE t = (Treap_##TYPE) malloc( sizeof(_Treap_pnode_##TYPE) ); \
  *t = NULL; \
  return t; \
} \
\
void \
_Treap_delete_##TYPE (_Treap_pnode_##TYPE v) \
{ \
  if (v != NULL) \
    { \
      _Treap_delete_##TYPE(v->l); \
      _Treap_delete_##TYPE(v->r); \
      free(v); \
    } \
} \
\
void \
delete_treap_##TYPE (Treap_##TYPE t) \
{ \
  _Treap_delete_##TYPE(*t); \
  free(t); \
} \
\
void \
split_treap_##TYPE (_Treap_pnode_##TYPE t, TYPE key, Treap_##TYPE l, \
                     Treap_##TYPE r) \
{ \
  if (t == NULL) \
    *l = *r = NULL; \
  else if (LESS(key, t->key)) \
    split_treap_##TYPE (t->l, key, l, &t->l),  *r = t; \
  else \
    split_treap_##TYPE (t->r, key, &t->r, r), *l = t; \
} \
\
void \
merge_treap_##TYPE (Treap_##TYPE t, _Treap_pnode_##TYPE l, \
                     _Treap_pnode_##TYPE r) \
{ \
  if ((l == NULL) || (r == NULL)) \
    *t = (l != NULL) ? l : r; \
  else if (l->priority > r->priority) \
    merge_treap_##TYPE (&l->r, l->r, r),  *t = l; \
  else \
    merge_treap_##TYPE (&r->l, l, r->l),  *t = r; \
} \
\
void \
_Treap_insert_##TYPE (Treap_##TYPE t, _Treap_pnode_##TYPE it) \
{ \
  if (*t == NULL) \
    *t = it; \
  else if (it->priority > (*t)->priority) \
    split_treap_##TYPE (*t, it->key, &it->l, &it->r),  *t = it; \
  else \
    _Treap_insert_##TYPE (LESS(it->key, (*t)->key) ? &(*t)->l : &(*t)->r, it); \
} \
\
void \
remove_treap_##TYPE (Treap_##TYPE t, TYPE key) \
{ \
  if (_Treap_##TYPE_equal((*t)->key, key)) \
    { \
      _Treap_pnode_##TYPE v = *t; \
      merge_treap_##TYPE (t, (*t)->l, (*t)->r); \
      free(v); \
    } \
  else \
    remove_treap_##TYPE (LESS(key, (*t)->key) ? &(*t)->l : &(*t)->r, key); \
} \
\
void \
insert_treap_##TYPE (Treap_##TYPE t, TYPE key) \
{ \
  _Treap_pnode_##TYPE it = (_Treap_pnode_##TYPE) \
                         malloc( sizeof(struct _Treap_struct_##TYPE) ); \
  it->key = key; \
  it->priority = rand(); \
  it->l = NULL; \
  it->r = NULL; \
  _Treap_insert_##TYPE(t, it); \
} \
\
bool \
in_tree_treap_##TYPE (Treap_##TYPE t, TYPE key) \
{ \
  _Treap_pnode_##TYPE v = *t; \
  while (v != NULL) \
    if (_Treap_##TYPE_equal(v->key, key)) \
      return true; \
    else \
      if (LESS(key, v->key)) \
        v = v->l; \
      else \
        v = v->r; \
  return false; \
} \
\
TYPE \
min_treap_##TYPE (Treap_##TYPE t) \
{ \
  _Treap_pnode_##TYPE v = *t; \
  while (v->l != NULL) \
    v = v->l; \
  return v->key; \
} \
\
TYPE \
max_treap_##TYPE (Treap_##TYPE t) \
{ \
  _Treap_pnode_##TYPE v = *t; \
  while (v->r != NULL) \
    v = v->r; \
  return v->key; \
} \
\
bool \
is_empty_treap_##TYPE (Treap_##TYPE t) \
{ \
  return *t == NULL; \
}

