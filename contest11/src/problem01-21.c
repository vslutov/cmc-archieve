#include <stdio.h>
#include <inttypes.h>

int
main(void)
{
  long long unsigned int A, B = 0, trit, sum, mass = 1;
  scanf("%llu", &A);
  sum = A;

  while (A > 0)
  {
    trit = A % 3;
    if (trit != 0)
    {
      ++B;
      sum += mass;
    }

    A /= 3;

    if (trit == 2)
      ++A;

    mass *= 3;
  }

  if (mass / 2 > 1000*1000)
    printf("-1\n");
  else
    printf("%llu\n", B);
  return 0;
}

