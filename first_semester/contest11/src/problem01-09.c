#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>

int
main(void) 
{
  uint32_t N, count = 0;
  scanf("%u", &N);
  while (N != 0)
  {
    ++count;
    N = N & (N - 1);
  }
  printf("%u\n", count);
  return 0;
}

