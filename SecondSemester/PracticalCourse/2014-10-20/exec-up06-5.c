#include "up06-5.c"

#include <stdio.h>

int
main(void) {
    RandomGenerator *rr = random_create(1234);

    for (int j = 0; j < 100; ++j) {
        printf("%d\n", rr->ops->next(rr));
    }

    rr->ops->destroy(rr);

    return 0;
}

