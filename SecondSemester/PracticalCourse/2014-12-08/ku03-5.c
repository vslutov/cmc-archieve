#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

int fd;
ssize_t size;

enum {
    CACHE_LINE_SIZE = 4,
    CACHE_LINE_COUNT = 8
};

typedef struct {
    int32_t data[CACHE_LINE_SIZE];
    ssize_t num;
    bool loaded, fixed;
} CacheLine;

CacheLine cache[CACHE_LINE_COUNT];

void
store (ssize_t line)
{
    ssize_t num = cache[line].num;

    if (cache[line].loaded == true && cache[line].fixed == true) {
        ssize_t s = sizeof(cache[line].data);
        lseek(fd, num * s, SEEK_SET);

        if (num == size / CACHE_LINE_COUNT) {
            s = (size % CACHE_LINE_SIZE) * sizeof(int32_t);
        }

        ssize_t w;
        char *buffer = (char *)&cache[line].data;
        while (s > 0 && (w = write(fd, buffer, s)) > 0) {
            s -= w;
            buffer += w;
        }
        cache[line].fixed = false;
    }
}

void
load (ssize_t num)
{
    ssize_t line = num % CACHE_LINE_COUNT;

    if (cache[line].num != num || cache[line].loaded != true) {
        store(line);

        ssize_t s = sizeof(cache[line].data);
        lseek(fd, num * s, SEEK_SET);

        ssize_t r;
        char *buffer = (char *)&cache[line].data;
        while (s > 0 && (r = read(fd, buffer, s)) > 0) {
            s -= r;
            buffer += r;
        }

        cache[line].num = num;
        cache[line].loaded = true;
        cache[line].fixed = false;
    }
}

void
cwrite (ssize_t i, int32_t value)
{
    ssize_t num = i / CACHE_LINE_SIZE;
    load(num);

    ssize_t line = num % CACHE_LINE_COUNT;
    cache[line].data[i % CACHE_LINE_SIZE] = value;
    cache[line].fixed = true;
}

int32_t
cread (ssize_t i)
{
    ssize_t num = i / CACHE_LINE_SIZE;
    load(num);

    ssize_t line = num % CACHE_LINE_COUNT;
    return cache[line].data[i % CACHE_LINE_SIZE];
}

void
sort(ssize_t low, ssize_t high)
{
    ssize_t i = low;
    ssize_t j = high;
    int32_t m = cread((i + j) / 2);

    do {
        while (cread(i) < m) {
            ++i;
        }
        while (cread(j) > m) {
            --j;
        }

        if (i <= j) {
            int32_t temp = cread(i);
            cwrite(i, cread(j));
            cwrite(j, temp);

            ++i;
            --j;
        }
    } while (i <= j);

    if (low < j) {
        sort(low, j);
    }

    if (i < high) {
        sort(i, high);
    }
}

int
main(int argc, char **argv)
{
    fd = open(argv[1], O_RDWR, 0777);

    ssize_t end = lseek(fd, 0, SEEK_END);
    size = end / sizeof(int32_t);

    sort(0, size - 1);

    for (ssize_t i = 0; i < CACHE_LINE_COUNT; ++i) {
        store(i);
    }
}
