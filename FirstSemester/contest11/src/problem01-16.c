#include <stdio.h>
#include <inttypes.h>

int
main(void)
{
  long double x, y, sum_of_x = 0, sum_of_y = 0;
  uint64_t i;
  for(i = 0; i < 3; ++i)
    {
      scanf("%Lf %Lf", &x, &y);
      sum_of_x += x;
      sum_of_y += y;
    }
  printf("%.4Lf %.4Lf\n", sum_of_x / 3.0, sum_of_y / 3.0);
}

