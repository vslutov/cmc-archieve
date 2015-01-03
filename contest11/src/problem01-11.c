#include<stdio.h>
#include<inttypes.h>

int
main (void)
{
  uint32_t x, MASK;
  scanf("%u", &x);
  MASK = ((1 << 8) - 1) << 24;
  printf("%u\n", x ^ MASK);
}
