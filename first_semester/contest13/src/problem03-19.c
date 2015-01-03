/*
    problem03-19 - python
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

    Python eats mice.
    Every K hours python gonna sleep.
    If python eats K ** m mice, he will sleep m ** 2 hours.

    How much time python sleep?
*/

#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <math.h>

const double EPS = 1e-9;

double
Abs (double A)
{
  return A < 0 ? -A : A;
}

bool
equal (double A, double B)
{
  return Abs(A - B) <= EPS;
}

bool
less (double A, double B)
{
  return B - A > EPS;
}

bool
less_or_equal (double A, double B)
{
  return less(A, B) || equal(A, B);
}

bool
more (double A, double B)
{
  return less(B, A);
}

bool more_or_equal (double A, double B)
{
  return less_or_equal(B, A);
}

uintmax_t
sqr (uintmax_t x)
{
  return x * x;
}

uintmax_t
power (uintmax_t x, uintmax_t t)
{
  if (t == 0)
    return 1;
  else
    if (t % 2 == 1)
      return power(x, t - 1) * x;
    else
      return sqr( power(x, t / 2) );
}

uintmax_t
time_to_sleep (uintmax_t catched_mice, double K)
{
  double amount;
  intmax_t order = 1;
  for (amount = K; less(amount, catched_mice); amount *= K)
    ++ order;

  return power(catched_mice, order);
}

int
main (void)
{
  double K;
  scanf("%lf", &K);

  double mouse_time = 0, last_mouse = 0;
  double wake_up = 0;
  double schedule = K;

  uintmax_t sleeped_hours = 0;
  uintmax_t catched_mice = 0;
  while (!equal(mouse_time, -1.0))
  {
    scanf("%lf", &mouse_time);
    if (!equal(mouse_time, -1.0))
    {
      if (more_or_equal(mouse_time, wake_up))
        {
          if (less_or_equal(schedule, mouse_time))
            {
              sleeped_hours += time_to_sleep(catched_mice, K);
              wake_up = schedule + time_to_sleep(catched_mice, K);
              catched_mice = 0;
            }

          if (less_or_equal(wake_up, mouse_time))
            {
              ++ catched_mice;
              schedule = (floor(mouse_time / K) + 1) * K;
            }
        }
      last_mouse = mouse_time;
    }
  }

  double result = sleeped_hours;
  if (less(last_mouse, wake_up))
    result -= wake_up - last_mouse;
  result /= last_mouse;
  result *= 100;

  printf("%.2lf\n", result);

  return 0;
}

