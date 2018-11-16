#include <stdio.h>
#include <inttypes.h>
#include <math.h>

int
main(void)
{
  const double EPS = 1e-12;
  double a, b, c, d, t, x, angle;
  scanf("%lf %lf %lf %lf", &a, &b, &c, &d);
  if (a < b)
    {
      t = a;
      a = b;
      b = t;
    }

  if (c < d)
    {
      t = c;
      c = d;
      d = t;
    }

  if (a >= c - EPS && b >= d - EPS)
    printf("YES\n");
  else
    {
      angle = 2*atan(d/c) + atan(sqrt(c*c + d*d - a*a)/a);
      x = sqrt(c*c + d*d) * sin(angle);

      if (x <= b + EPS)
        printf("YES\n");
      else
        printf("NO\n");
    }

  return 0;
}

