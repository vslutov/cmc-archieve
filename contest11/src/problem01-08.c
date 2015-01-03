#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>

int
main(void) 
{
  uint32_t N, K, mask, max = 0;
  scanf("%u %u", &N, &K);
  mask = (1 << K) - 1;
  while (N != 0)
  {
    if (max < (N & mask))
      max = N & mask;
    N >>= 1;
  }
  printf("%u\n", max);
  return 0;
}

