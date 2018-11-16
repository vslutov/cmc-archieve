#include "ElproConfig.h"

#include "profile.h"
#include "sum.h"

#include <stdlib.h>
#include <stdio.h>

enum {
    MATRIX_SIZE = 10000,
    PROFILE_ITERATIONS = 500
};

static float matrix[MATRIX_SIZE][MATRIX_SIZE];
static float global_sum;

static void
prepare(void)
{
    for (ssize_t i = 0; i < MATRIX_SIZE; ++ i) {
        for (ssize_t j = 0; j < MATRIX_SIZE; ++ j) {
            matrix[i][j] = (i + j) * (i + j);
        }
    }
}

static void
run_sum(void)
{
    global_sum = sum(matrix, MATRIX_SIZE);
}

static void
run_slow_sum(void)
{
    global_sum = slow_sum(matrix, MATRIX_SIZE);
}

int
main(void)
{
    printf("Elpro v%d.%d - elemental profiler\n",
           ELPRO_VERSION_MAJOR,
           ELPRO_VERSION_MINOR);
    printf("Optimization: -O%d\n", ELPRO_OPTIMIZATION_LEVEL);

    prepare();

    puts("Profiling: sum");
    char *report = profile(run_sum, PROFILE_ITERATIONS);
    puts(report);
    free(report);


    puts("Profiling: slow_sum");
    report = profile(run_slow_sum, PROFILE_ITERATIONS);
    puts(report);
    free(report);
    return 0;
}
