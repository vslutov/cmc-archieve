#include <stdio.h>
#include <inttypes.h>
#include <math.h>

int
main(void)
{
  long long x, y, t, A = 0;
  scanf("%lld %lld", &x, &y);

  if (x < 0)
    x = -x;
  if (y < 0)
    y = -y;
  if (x < y)
    {
      t = x;
      x = y;
      y = t;
    }

  A = 2 * x;

  if ((x - y) % 2 == 1)
    --A;

  printf("%lld\n", A);

  return 0;
}

