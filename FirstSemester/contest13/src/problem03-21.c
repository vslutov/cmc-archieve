/*
    problem03-21 - minimal interval cover
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

    Output minimal cover of some intervals.
*/

#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>

struct Interval
{
  intmax_t L, R;
};

void
print_interval (struct Interval A)
{
  printf("%jd %jd ", A.L, A.R);
}

struct Interval
scan_interval (void)
{
  struct Interval result;
  scanf("%jd %jd", &result.L, &result.R);
  return result;
}

void
sort (struct Interval *a, int L, int R)
{
  int i = L, j = R;
  struct Interval t;
  intmax_t m = a[(L + R) / 2].L;

  while (i <= j)
  {
    while (i < R && a[i].L < m) ++i;
    while (j > L && m < a[j].L) --j;

    if (i <= j)
      {
        t = a[i];
        a[i] = a[j];
        a[j] = t;
        ++ i;
        -- j;
      }
  }

  if (L < j) sort(a, L, j);
  if (i < R) sort(a, i, R);
}

intmax_t
max (intmax_t A, intmax_t B)
{
  return A >= B ? A : B;
}

int
main (void)
{
  size_t N, i;
  scanf("%zu", &N);

  struct Interval intervals [N];
  for (i = 0; i < N; ++i)
    intervals[i] = scan_interval();

  sort(intervals, 0, N - 1);

  struct Interval current_cover = intervals[0];
  for (i = 1; i < N; ++i)
    if (current_cover.R < intervals[i].L)
      {
        print_interval(current_cover);
        current_cover = intervals[i];
      }
    else
      current_cover.R = max(current_cover.R, intervals[i].R);

  print_interval(current_cover);
  printf("\n");

  return 0;
}

