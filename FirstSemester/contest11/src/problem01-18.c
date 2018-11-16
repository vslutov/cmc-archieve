#include <stdio.h>
#include <inttypes.h>

int
main(void)
{
  long long unsigned int A, B = 0, i, j, bit, N, M;
  scanf("%llx", &A);
  for (N = 0; N < 16; ++N)
    {
      bit = A & 1;
      A = A >> 1;
      j = 4 - (N / 4);
      i = 4 - (N % 4);
      M = 16 - j - 4*i + 4;
      B = B | (bit << M);
    }
  printf("%04llx\n", B);
  return 0;
}

