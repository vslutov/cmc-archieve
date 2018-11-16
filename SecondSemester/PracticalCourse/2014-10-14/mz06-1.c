#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char **argv)
{
    intmax_t input;

    sscanf(argv[1], "%jd", &input);
    int32_t count = input;

    sscanf(argv[2], "%jd", &input);
    int32_t low = input;

    sscanf(argv[3], "%jd", &input);
    int32_t high = input;

    sscanf(argv[4], "%jd", &input);
    int32_t seed = input;

    srand(seed);

    for (ssize_t i = 0; i < count; ++i) {
        double U = (double)rand() / RAND_MAX;
        int32_t N = high - low;
        int32_t v = (int32_t)(U * N + low);

        printf("%jd\n", (intmax_t)v);
    }

    return 0;
}
