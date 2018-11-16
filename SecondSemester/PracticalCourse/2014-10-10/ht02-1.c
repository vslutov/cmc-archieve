#include <stdbool.h>
#include <stdio.h>
#include <inttypes.h>

enum {
    MAX_SIZE = 1000
};

int
main(int argc, char **argv)
{
    size_t size;
    sscanf(argv[1], "%zu", &size);

    intmax_t array[MAX_SIZE];
    size_t tail = 0;
    bool buffer_full = false;

    while (scanf("%jd", &array[tail]) == 1) {
        tail = (tail + 1) % size;
        if (tail == 0) {
            buffer_full = true;
        }
    }

    if (buffer_full) {
        for (size_t i = 0; i < size; ++i) {
            printf("%jd ", array[(tail + i) % size]);
        }
    } else {
        for (size_t i = 0; i < tail; ++i) {
            printf("%jd ", array[i]);
        }
    }

    printf("\n");

    return 0;
}

