/*
    problem03-13 - sequences
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

    Output all ascending K-length sequences of element from [1..N]
*/

#include <stdio.h>

void
recursive_output(size_t i, size_t last_used, size_t *a, size_t N, size_t K)
{
  size_t j;
  if (i == K)
    {
      for (j = 0; j < K; ++j)
        printf("%zu ", a[j]);
      printf("\n");
    }
  else
    for (j = last_used + 1; j < N; ++j)
      if (N - j >= K - i) // Could we complete current sequence with remaining
                          // numbers?
        {
          a[i] = j;
          recursive_output(i+1, j, a, N, K);
        }
}

int
main (void)
{
  size_t N, K;
  scanf("%zu %zu", &N, &K);
  size_t a[K];
  recursive_output(0, -1, a, N, K);
  return 0;
}

