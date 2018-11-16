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
typedef struct _Treap_##TYPE_struct { \
  TYPE key; \
  int priority; \
  struct _Treap_##TYPE_struct *l, *r; \
} *_Treap_##TYPE_pnode, **Treap_##TYPE; \
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
  Treap_##TYPE t = (Treap_##TYPE) malloc( sizeof(_Treap_##TYPE_pnode) ); \
  *t = NULL; \
  return t; \
} \
\
void \
_Treap_##TYPE_delete (_Treap_##TYPE_pnode v) \
{ \
  if (v != NULL) \
    { \
      _Treap_##TYPE_delete(v->l); \
      _Treap_##TYPE_delete(v->r); \
      free(v); \
    } \
} \
\
void \
delete_treap_##TYPE (Treap_##TYPE t) \
{ \
  _Treap_##TYPE_delete(*t); \
  free(t); \
} \
\
void \
_Treap_##TYPE_split (_Treap_##TYPE_pnode t, TYPE key, Treap_##TYPE l, \
                     Treap_##TYPE r) \
{ \
  if (t == NULL) \
    *l = *r = NULL; \
  else if (LESS(key, t->key)) \
    _Treap_##TYPE_split (t->l, key, l, &t->l),  *r = t; \
  else \
    _Treap_##TYPE_split (t->r, key, &t->r, r), *l = t; \
} \
\
void \
_Treap_##TYPE_merge (Treap_##TYPE t, _Treap_##TYPE_pnode l, \
                     _Treap_##TYPE_pnode r) \
{ \
  if ((l == NULL) || (r == NULL)) \
    *t = (l != NULL) ? l : r; \
  else if (l->priority > r->priority) \
    _Treap_##TYPE_merge (&l->r, l->r, r),  *t = l; \
  else \
    _Treap_##TYPE_merge (&r->l, l, r->l),  *t = r; \
} \
\
void \
_Treap_##TYPE_insert (Treap_##TYPE t, _Treap_##TYPE_pnode it) \
{ \
  if (*t == NULL) \
    *t = it; \
  else if (it->priority > (*t)->priority) \
    _Treap_##TYPE_split (*t, it->key, &it->l, &it->r),  *t = it; \
  else \
    _Treap_##TYPE_insert (LESS(it->key, (*t)->key) ? &(*t)->l : &(*t)->r, it); \
} \
\
void \
remove_treap_##TYPE (Treap_##TYPE t, TYPE key) \
{ \
  if (_Treap_##TYPE_equal((*t)->key, key)) \
    { \
      _Treap_##TYPE_pnode v = *t; \
      _Treap_##TYPE_merge (t, (*t)->l, (*t)->r); \
      free(v); \
    } \
  else \
    remove_treap_##TYPE (LESS(key, (*t)->key) ? &(*t)->l : &(*t)->r, key); \
} \
\
void \
insert_treap_##TYPE (Treap_##TYPE t, TYPE key) \
{ \
  _Treap_##TYPE_pnode it = (_Treap_##TYPE_pnode) \
                         malloc( sizeof(struct _Treap_##TYPE_struct) ); \
  it->key = key; \
  it->priority = rand(); \
  it->l = NULL; \
  it->r = NULL; \
  _Treap_##TYPE_insert(t, it); \
} \
\
bool \
in_tree_treap_##TYPE (Treap_##TYPE t, TYPE key) \
{ \
  _Treap_##TYPE_pnode v = *t; \
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
  _Treap_##TYPE_pnode v = *t; \
  while (v->l != NULL) \
    v = v->l; \
  return v->key; \
} \
\
TYPE \
max_treap_##TYPE (Treap_##TYPE t) \
{ \
  _Treap_##TYPE_pnode v = *t; \
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

