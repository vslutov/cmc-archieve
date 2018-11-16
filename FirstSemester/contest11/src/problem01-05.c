#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>

int
main(void) 
{
  bool odd = false;
  int64_t sum, last_elem = 0, now_elem;
  scanf("%lld", &sum);

  do 
    {
      scanf("%lld", &now_elem);
      
      if (now_elem != 0)
        {
          if (odd)
            sum += last_elem * now_elem;
    
          last_elem = now_elem;
          odd = !odd;
        }
    }
  while (now_elem != 0);

  if (odd)
    sum += last_elem;

  printf("%lld\n", sum);
  return 0;
}

