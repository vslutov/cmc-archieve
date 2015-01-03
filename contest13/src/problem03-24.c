/*
    problem03-24 - flashlight beams
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

    Flashlight defer by one number.

    Flashlight illuminate A cells at the right of self if A > 0.
    And |A| cells at the left if A < 0.

    Flashlight can't cover cells if another flashlight block.

    How many cells are covered?
*/

#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

intmax_t
min (intmax_t A, intmax_t B)
{
  return A <= B ? A : B;
}

intmax_t
max (intmax_t A, intmax_t B)
{
  return A >= B ? A : B;
}

intmax_t
lighten (intmax_t N)
{
  intmax_t result = 0;
  intmax_t last_flashlight = -1, last_luminescence = 0;
  bool covered_flashlight = true;

  intmax_t current_luminescence;
  intmax_t i;
  for (i = 0; i < N; ++i)
    {
      scanf("%jd", &current_luminescence);
      if (current_luminescence != 0)
        {
          intmax_t length = i - last_flashlight - 1;

          if (current_luminescence < 0)
            {
              result += min(-current_luminescence, length);
              if ((-current_luminescence >= i - last_flashlight) &&
                  !covered_flashlight)
                ++ result;

              if (last_luminescence > 0)
                result -= max( min(last_flashlight+last_luminescence, i-1)
                              -max(i+current_luminescence, last_flashlight+1)
                               + 1, 0);
            }

          if (last_luminescence > 0)
            result += min(last_luminescence, length);

          if (last_luminescence >= i - last_flashlight)
            {
              ++ result;
              covered_flashlight = true;
            }
          else
            covered_flashlight = false;

          last_luminescence = current_luminescence;
          last_flashlight = i;
        }
    }

  if (last_luminescence > 0)
    result += min(last_luminescence, N - 1 - last_flashlight);

  return result;
}

int
main (void)
{
  intmax_t N;
  scanf("%jd", &N);
  printf("%jd\n", lighten(N));
  return 0;
}

