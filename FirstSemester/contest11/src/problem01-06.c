#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>

int
main(void) 
{
  uint32_t N, K;
  scanf("%u %u", &N, &K);
  N = (N >> K) | ((N & ((1 << K) - 1)) << (32 - K));
  printf("%u\n", N);
  return 0;
}

