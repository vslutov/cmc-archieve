#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>

struct Data
{
    int32_t x;
    int64_t y;
};

enum {
    DATA_SIZE = 12
};

void
read_bytes(int fd, void *input, int size)
{
    char *buffer = (char *) input;
    int r;

    while (size > 0 && (r = read(fd, buffer, size)) > 0) {
        buffer += r;
        size -= r;
    }

    assert(r >= 0);
    assert(size == 0);
}

void
write_bytes(int fd, void *input, int size)
{
    char *buffer = (char *) input;
    int w;

    while (size > 0 && (w = write(fd, buffer, size)) > 0) {
        buffer += w;
        size -= w;
    }

    assert(w >= 0);
    assert(size == 0);
}

void
read_data(int fd, int index, struct Data *elem)
{
    lseek(fd, index * DATA_SIZE, SEEK_SET);

    read_bytes(fd, &(elem->x), sizeof(elem->x));
    read_bytes(fd, &(elem->y), sizeof(elem->y));
}

void
write_data(int fd, int index, struct Data *elem)
{
    lseek(fd, index * DATA_SIZE, SEEK_SET);

    write_bytes(fd, &(elem->x), sizeof(elem->x));
    write_bytes(fd, &(elem->y), sizeof(elem->y));
}

int
main(int argc, char **argv)
{
    int fd = open(argv[1], O_RDWR, 0);
    intmax_t a;
    sscanf(argv[2], "%jd", &a);

    int size = lseek(fd, 0, SEEK_END);
    assert(size >= 0);
    size /= DATA_SIZE;

    for (int i = 0; 2 * i < size; ++i) {
        struct Data first_elem, second_elem;

        read_data(fd, i, &first_elem);
        read_data(fd, size - 1 - i, &second_elem);

        first_elem.y += ((intmax_t) first_elem.x) * a;
        second_elem.y += ((intmax_t) second_elem.x) * a;

        write_data(fd, i, &second_elem);
        write_data(fd, size - 1 - i, &first_elem);
    }

    return 0;
}

