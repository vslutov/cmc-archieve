#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <time.h>

int
main(int argc, char **argv)
{
    FILE *fin = fopen(argv[1], "r");

    struct tm current;
    time_t current_time;
    time_t prev_time = -1;

    while (fscanf(
        fin,
        "%d/%d/%d %d:%d:%d",
        &current.tm_year,
        &current.tm_mon,
        &current.tm_mday,
        &current.tm_hour,
        &current.tm_min,
        &current.tm_sec) == 6) {

        current.tm_year -= 1900;
        current.tm_mon -= 1;

        current.tm_isdst = -1;

        current_time = mktime(&current);

        if (prev_time != -1) {
            printf("%jd\n", (intmax_t)current_time - (intmax_t)prev_time);
        }

        prev_time = current_time;
    }

    fclose(fin);

    return 0;
}
