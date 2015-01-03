/*
    problem04-02 - Haiku
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

    Check sequence of strings for right syllable allocation.
*/

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

const size_t MAX_LENGTH = 100;

bool
is_vowel(char c)
{
  if (('A' <= c) && (c <= 'Z'))
    c += 'a' - 'A';
  return ((c == 'a') || (c == 'e') || (c == 'i') || (c == 'o') || (c == 'u')
          || (c == 'y')) ? true : false;
}

size_t
syllables (const char *s)
{
  size_t result;

  result = is_vowel(s[0]) ? 1 : 0;

  size_t i = 0;
  while (s[i] != 0)
  {
    ++ i;
    if (is_vowel(s[i]) && (!is_vowel(s[i-1])))
      ++ result;
  }

  return result;
}

int
main (void)
{
  size_t N;
  scanf("%zu", &N);

  size_t syllables_counts[N];
  for (size_t i = 0; i < N; ++i)
    scanf("%zu", &syllables_counts[i]);
  scanf("\n");

  char s[MAX_LENGTH + 1];

  for (size_t i = 0; i < N; ++i)
    {
      gets(s);
      if (syllables(s) == syllables_counts[i])
        puts(s);
    }

  return 0;
}

