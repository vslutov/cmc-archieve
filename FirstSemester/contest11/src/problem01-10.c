#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>

int
main(void) 
{
  uint32_t N, a, b, c, d;
  scanf("%u %u %u %u", &a, &b, &c, &d);
  N = (d << 24) | (c << 16) | (b << 8) | (a << 0);
  printf("%u\n", N);
  return 0;
}

