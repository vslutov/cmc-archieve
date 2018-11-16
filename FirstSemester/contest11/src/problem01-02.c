#include <stdio.h>
#include <inttypes.h>

int main(void) {
  long long unsigned N, i, count_max;
  long long int a, max;
  scanf("%llu %lld", &N, &a);
  max = a;
  count_max = 1;
  for(i = 1; i < N; ++i) {
    scanf("%lld", &a);
    if(max < a) {
      max = a;
      count_max = 1;
    } else if(max == a)
      ++count_max;
  }
  printf("%llu\n", count_max);
  return 0;
}

