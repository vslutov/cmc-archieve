#include<stdio.h>
#include<inttypes.h>
#include<stdbool.h>

int
main (void)
{
  bool convex = true;
  uint64_t N, i;
  int64_t x, y, Vx, Vy, last_x, last_y, last_Vx, last_Vy, m, last_m;

  scanf("%llu %lld %lld %lld %lld %lld %lld", &N, &last_Vx, &last_Vy, &last_x, &last_y, &x, &y);

  last_Vx = last_x - last_Vx;
  last_Vy = last_y - last_Vy;

  Vx = x - last_x;
  Vy = y - last_y;

  last_m = last_Vx * Vy + last_Vy * Vx;

  for (i = 3; i < N; ++i)
    {
      last_x = x;
      last_y = y;

      scanf("%lld %lld", &x, &y); 

      Vx = x - last_x;
      Vy = y - last_y;


      m = last_Vx * Vy + last_Vy * Vx;

      if (m * last_m < 0)
        convex = false;
     }

  if (convex)
    printf("Yes\n");
  else
    printf("No\n");

  return 0;
}
