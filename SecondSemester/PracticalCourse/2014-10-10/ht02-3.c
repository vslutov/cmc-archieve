#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
    if (argc != 4) {
        goto arg_fail;
    }

    char *path = argv[1];
    if (strlen(path) > PATH_MAX) {
        goto arg_fail;
    }

    uintmax_t input_offset;
    int finish;
    int result = sscanf(argv[2], "%ju%n", &input_offset, &finish);
    uint32_t offset = input_offset;
    if (result != 1 || finish != strlen(argv[2]) || input_offset > LONG_MAX) {
        goto arg_fail;
    }

    intmax_t input_size;
    result = sscanf(argv[3], "%jd%n", &input_size, &finish);
    size_t size = input_size;
    if (result != 1 ||
        finish != strlen(argv[3]) ||
        input_size < 1 ||
        input_size > 32) {

        goto arg_fail;
    }

    int rfd = open(path, O_RDONLY, 0777);
    if (rfd < 0) {
        goto inp_fail;
    }

    off_t byte_offset = offset / 8;
    result = lseek(rfd, byte_offset, SEEK_SET);
    if (result != byte_offset) {
        goto inp_fail;
    }

    uint32_t end = offset + size;

    size_t tail = end % 8;
    size_t head = offset % 8;

    size_t byte_count = end / 8 + (tail == 0 ? 0 : 1) - byte_offset;

    uintmax_t answer;
    result = read(rfd, &answer, byte_count);
    if (result != byte_count) {
        goto inp_fail;
    }

    answer >>= head;
    answer &= (((uintmax_t) 1) << size) - 1;
    printf("%ju\n", answer);

    close(rfd);

    return 0;

arg_fail:
    fprintf(stderr, "Invalid arguments\n");
    return 1;

inp_fail:
    fprintf(stderr, "Input error\n");
    close(rfd);
    return 1;
}
