/*
    problem03-16 - superpower
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

    super_power(a, b, c) := power(a, power(b, c))

    get out max super_power(x, y, z) mod m
      where {x, y, z} = {a, b, c}
*/

#include <stdio.h>
#include <inttypes.h>

uintmax_t
max (uintmax_t a, uintmax_t b)
{
  if (a >= b)
    return a;
  else
    return b;
}

uintmax_t
sqr (uintmax_t x)
{
  return x * x;
}

uintmax_t
sqr_mod_m (uintmax_t x, uintmax_t m)
{
  return (x * x) % m;
}

uintmax_t
power (uintmax_t x, uintmax_t t)
{
  if (t == 0)
    return 1;
  else if (t % 2 == 0)
    return sqr(power(x, t / 2));
  else // if (t % 2 == 1)
    return power(x, t - 1) * x;
}

uintmax_t
power_mod_m (uintmax_t x, uintmax_t t, uintmax_t m)
{
  if (t == 0)
    return 1 % m;
  else if (t % 2 == 0)
    return sqr_mod_m(power_mod_m(x, t / 2, m), m);
  else // if (t % 2 == 1)
    return (power_mod_m(x, t - 1, m) * x) % m;
}

uintmax_t
super_power (uintmax_t a, uintmax_t b, uintmax_t c, uintmax_t m)
{
  return power_mod_m(a, power(b, c), m);
}

int
main (void)
{
  uintmax_t m;
  uintmax_t a[3];
  scanf("%ju %ju %ju %ju", &a[0], &a[1], &a[2], &m);

  uintmax_t result = 0;
  size_t i, j, k;
  for (i = 0; i < 3; ++i)
    for (j = 0; j < 3; ++j)
      for (k = 0 ; k < 3; ++k)
        if ((i != j) && (j != k) && (i != k))
            result = max(result, super_power(a[i], a[j], a[k], m));

  printf("%ju\n", result);
  return 0;
}

