#include<stdio.h>
#include<inttypes.h>

int
main (void)
{
  uint64_t N, i;
  int64_t x, max1, max2, min1, min2;

  scanf("%llu %lld %lld", &N, &min1, &min2);
  
  if (min1 > min2)
    {
      x = min1;
      min1 = min2;
      min2 = x;
    }

  max1 = min2;
  max2 = min1;

  for (i = 2; i < N; ++i)
    {
      scanf("%lld", &x);
      
      if (x >= max1)
        {
          max2 = max1;
          max1 = x;
        }
      else if (x >= max2)
        max2 = x;

      if (x <= min1)
        {
          min2 = min1;
          min1 = x;
        }
      else if (x <= min2)
        min2 = x;

    }

  if (max1 * max2 <= min1 * min2)
    printf("%lld %lld\n", min1, min2);
  else
    printf("%lld %lld\n", max2, max1);

  return 0;
}
