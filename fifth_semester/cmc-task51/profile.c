#include "profile.h"
#include "op_rdtsc.h"

#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

enum {
    STR_START_SIZE = 5000
};

char *
profile(VoidFunction test_function, ssize_t iterations)
{
    ++ iterations; // first value hack

    clock_t *times = malloc(iterations * sizeof(*times));
    clock_t last_time;

    #ifdef HAVE_RDTSC
    uint64_t *tacts = malloc(iterations * sizeof(*tacts));
    uint64_t last_tacts = op_rdtsc();
    #endif // HAVE_RDTSC

    last_time = clock();
    for (ssize_t i = 0; i < iterations; ++ i) {
        test_function();
        times[i] = clock() - last_time;
        last_time = times[i] + last_time;

        #ifdef HAVE_RDTSC
        tacts[i] = op_rdtsc() - last_tacts;
        last_tacts = tacts[i] + last_tacts;
        #endif // HAVE_RDTSC
    }

    double average_time = 0;
    for (ssize_t i = 1; i < iterations; ++ i) {
        average_time += times[i];
    }
    average_time /= iterations - 1;

    double disp_time = 0;
    for (ssize_t i = 1; i < iterations; ++ i) {
        disp_time += (times[i] - average_time) * (times[i] - average_time);
    }

    free(times);
    average_time /= CLOCKS_PER_SEC;
    disp_time /= CLOCKS_PER_SEC * CLOCKS_PER_SEC * (iterations - 2);

    #ifdef HAVE_RDTSC
    double average_tacts = 0;
    for (ssize_t i = 1; i < iterations; ++ i) {
        average_tacts += tacts[i];
    }
    average_tacts /= iterations - 1;

    double disp_tacts = 0;
    for (ssize_t i = 1; i < iterations; ++ i) {
        disp_tacts += (tacts[i] - average_tacts) * (tacts[i] - average_tacts);
    }

    free(tacts);
    disp_time /= (iterations - 2);
    #endif // HAVE_RDTSC

    char *str_head = malloc(STR_START_SIZE);
    char *str_tail = str_head;
    ssize_t str_size = STR_START_SIZE;

    snprintf(str_tail,
             str_size,
             "Average execution time: %.2fs (+/- %.2fs).\n",
             average_time,
             6 * sqrt(disp_time));

    str_tail = str_head + strlen(str_head);
    str_size = STR_START_SIZE - strlen(str_head);

    #ifdef HAVE_RDTSC
    snprintf(str_tail,
             str_size,
             "Average execution tacts: %.0f (+/- %.0f).",
             average_tacts,
             6 * sqrt(disp_tacts));
    #else // HAVE_RDTSC
    snprintf(str_tail,
             str_size,
             "Tact counter wasn't compiled.");
    #endif // HAVE_RDTSC

    str_head = realloc(str_head, strlen(str_head));
    return str_head;
}
