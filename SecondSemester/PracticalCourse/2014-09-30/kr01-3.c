#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>

struct Entity
{
    unsigned short size;
    long long offset;
};

struct Entity
get_entity(int fd, long long offset)
{
    struct Entity result;

    char *buffer = (char *) &result.size;
    int size = sizeof(result.size);

    lseek(fd, offset, SEEK_SET);

    int r;

    while (size > 0 && (r = read(fd, buffer, size)) > 0) {
        size -= r;
        buffer += r;
    }

    if (r == 0) {
        printf("0\n0\n");
        exit(0);
    }

    lseek(fd, result.size, SEEK_CUR);

    buffer = (char *) &result.offset;
    size = sizeof(result.offset);

    r = 0;

    while (size > 0 && (r = read(fd, buffer, size)) > 0) {
        size -= r;
        buffer += r;
    }

    return result;
}

int
main(int argc, char **argv)
{
    int fdr = open(argv[1], O_RDONLY, 0);

    long long offset;
    sscanf(argv[2], "%lld", &offset);

    struct Entity e;

    long long sum_size = 0;
    size_t count = 0;

    while (offset != -1) {
        e = get_entity(fdr, offset);
        sum_size += e.size;
        if (e.offset == 0) {
            offset = -1;
        } else {
            offset += e.offset;
        }
        ++count;
    }

    printf("%zu\n%lld\n", count, sum_size);

    return 0;
}

