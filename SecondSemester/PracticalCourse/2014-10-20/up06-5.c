#include <stdlib.h>
#include <inttypes.h>

enum {
    A = 1103515245,
    C = 12345,
    M = 1 << 31
};

typedef struct RandomGenerator RandomGenerator;

struct RandomOperations
{
    void (*destroy)(RandomGenerator *rr);
    int (*next)(RandomGenerator *rr);
};

struct RandomGenerator
{
    const struct RandomOperations *ops;
    int value;
};

void
random_destroy(RandomGenerator *rr)
{
    free(rr);
}

int
random_next(RandomGenerator *rr)
{
    intmax_t value = rr->value;
    intmax_t next = (A * value + C) % M;

    rr->value = next;
    return next;
}

const struct RandomOperations random_operations = {
    random_destroy,
    random_next
};

RandomGenerator *
random_create(int seed)
{
    RandomGenerator *result = calloc(1, sizeof(*result));
    result->value = seed;

    result->ops = &random_operations;

    return result;
}

