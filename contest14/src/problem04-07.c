/*
    problem04-07 - towns
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

    Output most chain of words, if last letter of word == first letter of the
    next one.
*/

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 20
#define MAX_N 10

typedef struct word Word;

struct word
{
  char name[MAX_LENGTH + 1];
  char first_letter;
  char last_letter;
  size_t count_edges;
  Word *edges[MAX_N];
  bool used;
  size_t max_length_from;
  size_t max_length_to;
};

size_t
max (size_t a, size_t b)
{
  return a < b ? b : a;
}

void
dfs (Word *v)
{
  v->used = true;
  for (size_t i = 0; i < v->count_edges; ++i)
    {
      Word *u = v->edges[i];
      if ((!u->used) && (u->max_length_to < v->max_length_to + 1))
        {
          u->max_length_to = v->max_length_to + 1;
          dfs(u);
        }
    }
  v->used = false;
}

void
calc_max_chain_length (size_t v, size_t n, Word *towns)
{
  for (size_t i = 0; i < n; ++i)
  {
    towns[i].max_length_to = 1;
    towns[i].used = false;
  }

  dfs(towns + v);

  size_t result = 1;
  for (size_t i = 0; i < n; ++i)
    result = max(result, towns[i].max_length_to);

  towns[v].max_length_from = result;
}

int
main (void)
{
  size_t n;
  scanf("%u", &n);

  Word towns[n];
  assert(towns != NULL);

  for (size_t i = 0; i < n; ++i)
  {
    scanf("%s", towns[i].name);
    size_t length = strlen(towns[i].name);
    towns[i].first_letter = towns[i].name[0];
    towns[i].last_letter = towns[i].name[length - 1];
  }

  for (size_t i = 0; i < n; ++i)
  {
    towns[i].count_edges = 0;
    for (size_t j = 0; j < n; ++j)
      if (towns[i].last_letter == towns[j].first_letter)
        {
          size_t new_edge = towns[i].count_edges ++;
          towns[i].edges[new_edge] = towns + j;
        }
  }

  size_t max_chain_length = 0;

  for (size_t i = 0; i < n; ++i)
    {
      calc_max_chain_length(i, n, towns);
      max_chain_length = max(max_chain_length, towns[i].max_length_from);
    }

  printf("%u\n", max_chain_length);

  for (size_t i = 0; i < n; ++i)
    if (towns[i].max_length_from == max_chain_length)
      printf("%s\n", towns[i].name);

  return 0;
}
