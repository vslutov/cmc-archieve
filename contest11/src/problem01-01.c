#include <stdio.h>
#include <inttypes.h>

int main(void) {
  long long unsigned N, i;
  long long int a, min, max;
  scanf("%llu %lld", &N, &a);
  min = max = a;
  for(i = 1; i < N; ++i) {
    scanf("%lld", &a);
    if(max < a) max = a;
    if(min > a) min = a;
  }
  printf("%lld\n", max - min);
  return 0;
}

