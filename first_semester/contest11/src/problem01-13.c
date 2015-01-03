#include<stdio.h>
#include<inttypes.h>

int
main (void)
{
  double x, sin, dsin;
  uint64_t N, i;

  scanf("%lf %llu", &x, &N);

  dsin = sin = x;

  for (i = 1; i < N; ++i)
    {
      dsin = (-1.0) * dsin * x * x / (2 * i) / (2 * i + 1);
      sin += dsin;
    }

  printf("%.6lf\n", sin);

  return 0;
}
