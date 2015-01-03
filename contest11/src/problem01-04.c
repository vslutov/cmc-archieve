#include <stdio.h>
#include <inttypes.h>

int
main(void) {
  uint64_t N, i, length_increasing_sequence;
  uint64_t max_length_increasing_sequence;
  scanf("%llu", &N);

  int64_t last_elem, now_elem;
  scanf("%lld", &last_elem);

  max_length_increasing_sequence = length_increasing_sequence = 1;

  for(i = 1; i < N; ++i)
  {
    scanf("%lld", &now_elem);
    
    if (now_elem > last_elem)
      {
        ++length_increasing_sequence;
        if (max_length_increasing_sequence < length_increasing_sequence)
          max_length_increasing_sequence = length_increasing_sequence;
      }
    else
      length_increasing_sequence = 1;

    last_elem = now_elem;
  }

  printf("%llu\n", max_length_increasing_sequence);
  return 0;
}

