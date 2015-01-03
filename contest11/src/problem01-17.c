#include <stdio.h>
#include <inttypes.h>

int
main(void)
{
  long long unsigned int N, i, x, unique = 0;
  scanf("%llu", &N);
  for (i = 0; i < N; ++i)
    {
      scanf("%llu", &x);
      unique ^= x;
    }
  printf("%llu\n", unique);
  return 0;
}

