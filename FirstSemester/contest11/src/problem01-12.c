#include<stdio.h>
#include<inttypes.h>

int
main (void)
{
  uint64_t N, i;
  int64_t x, m1, m2, m3;

  scanf("%llu %lld %lld %lld", &N, &m1, &m2, &m3);
  
  if (m1 < m2)
    {
      x = m1;
      m1 = m2;
      m2 = x;
    }
  
  if (m1 < m3)
    {
      x = m1;
      m1 = m3;
      m3 = x;
    }

  if (m2 < m3)
    {
      x = m2;
      m2 = m3;
      m3 = x;
    }

  for(i = 3; i < N; ++i)
    {
      scanf("%lld", &x);

      if (x >= m1)
        {
          m3 = m2;
          m2 = m1;
          m1 = x;
        }
      else if (x >= m2)
        {
          m3 = m2;
          m2 = x;
        }
      else if (x >= m3)
        m3 = x;

    }

  printf("%lld %lld %lld\n", m1, m2, m3);
}
