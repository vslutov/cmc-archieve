#include <stdio.h>
#include <inttypes.h>

const int K = (int)('z') - (int)('a') + 1;

int
char_to_number(int a)
{
  if ((int)('a') <= a && a <= (int)('z'))
    return a - (int)('a');
  else
    return a - (int)('A') + K;
}

int
number_to_char(int x)
{
  if (x < K)
    return x + (int)('a');
  else
    return x - K + (int)('A');
}

int
main(void) {
  int N, a;
  scanf("%d", &N);

  do 
    {
      a = getchar();
      if (a == ' ' || a == '.')
        {
          putchar(a);
        }
      else if (a != '.')
        {
          int x, y;
          x = char_to_number(a);
          y = x / K;
          x = (x + N) % K + y * K;
          a = number_to_char(x);
          putchar(a);
        }
    }
  while(a != '.');

  putchar('\n');
  return 0;
}

