#include "rand.h"

#include <inttypes.h>
#include <stdlib.h>

int
rand_range(int low, int high)
{
    double U = (double)rand() / RAND_MAX;
    int32_t N = high - low;
    return (int32_t)(U * N + low);
}
