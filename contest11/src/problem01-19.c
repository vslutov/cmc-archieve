#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>

int
main(void)
{
  uint32_t a11, a12, a21, a22, b1, b2, x = 0, y = 0;
  uint64_t N;
  uint32_t A11, A12, A21, A22, B1, B2, X, Y;
  bool has_answer;
  scanf("%u %u", &a11, &a12);
  scanf("%u %u", &a21, &a22);
  scanf("%u %u", &b1, &b2);

  for (N = 0; N < 32; ++N)
    {
      A11 = (a11 >> N) & 1;
      A12 = (a12 >> N) & 1;
      A21 = (a21 >> N) & 1;
      A22 = (a22 >> N) & 1;
      B1 = (b1 >> N) & 1;
      B2 = (b2 >> N) & 1;

      has_answer = false;
      for (X = 0; (!has_answer) && (X < 2); ++X)
        for (Y = 0; (!has_answer) && (Y < 2); ++Y)
          if ( (((A11 & X) ^ (A12 & Y)) == B1) &&
               (((A21 & X) ^ (A22 & Y)) == B2) )
              {
                x = x | (X << N);
                y = y | (Y << N);
                has_answer = true;
              }

      if (!has_answer)
        {
          printf("No\n");
          return 0;
        }
    }

  printf("Yes\n%u %u\n", x, y);

  return 0;
}

