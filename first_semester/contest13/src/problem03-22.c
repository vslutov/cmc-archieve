/*
    problem03-22 - lucky numbers
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

    X is lucky if sum(X[i] for [1..N/2]) = sum(X[i] for [N/2+1..N]),
      where N is count of digit.

    Output minimal number more or equal then A.
*/

#include <stdio.h>
#include <string.h>

int
min (int A, int B)
{
  return A <= B ? A : B;
}

int
main (void)
{
  char s[81];
  scanf("%s", s);

  int N = strlen(s);
  int i;

  if (N % 2 == 1)
    {
      for (i = N + 2; i > 0; -- i)
        s[i] = s[i-1];
      s[0] = '0';
      N = N / 2 + 1;
    }
  else
    N = N / 2;

  int left_sum = 0, right_sum = 0;

  for (i = 0; i < N; ++i)
    left_sum += (int)s[i] - (int)'0';

  for (i = N; i < 2 * N; ++i)
    right_sum += (int)s[i] - (int)'0';

  int factor;

  for (i = 2 * N - 1; i >= 0; --i)
    if ((left_sum < right_sum) && (s[i] != '0'))
      {
        right_sum -= (int)s[i] - (int)'0';
        s[i] = '0';
        int j;
        for (j = i - 1; s[j] == '9'; --j)
          {
            if (j >= N)
              right_sum -= 9;
            else
              left_sum -= 9;
            s[j] = '0';
          }
        if (j >= N)
          ++ right_sum;
        else
          ++ left_sum;
        s[j] += 1;
      }

  for (i = 2 * N - 1; i >= 0; -- i)
    if (left_sum > right_sum)
      {
        factor = min(left_sum - right_sum, (int)'9' - (int)s[i]);
        right_sum += factor;
        s[i] += factor;
      }

  if (s[0] == '0')
    printf("%s\n", s+1);
  else
    printf("%s\n", s);

  return 0;
}

