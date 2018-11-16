#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char **argv)
{
    int result;

    ssize_t count;
    result = scanf("%zd", &count);
    assert(result == 1);


    intmax_t distribution[100];
    ssize_t filled = 0;

    for (ssize_t i = 0; i < count; ++i) {
        intmax_t value;
        ssize_t probability;
        result = scanf("%jd %zd", &value, &probability);
        assert(result == 2);

        for (ssize_t j = 0; j < probability; ++j) {
            distribution[filled] = value;
            ++filled;
        }
    }

    intmax_t seed;
    result = scanf("%zd %jd", &count, &seed);
    assert(result == 2);

    srand(seed);

    for (ssize_t i = 0; i < count; ++i) {
        double U = (double)rand() / RAND_MAX;
        ssize_t j = (ssize_t)(U * 100);

        printf("%jd\n", distribution[j]);
    }

    return 0;
}
