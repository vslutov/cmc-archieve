#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

enum {
    BUFFER_SIZE = 16,
    INT_BUFFER_SIZE = 64
};

int
get_char(void)
{
    static char buffer[BUFFER_SIZE];
    static int pointer = 0;
    static int buf_size = 0;

    if (pointer == buf_size) {
        pointer = 0;
        buf_size = read(0, buffer, sizeof(buffer));
        assert(buf_size >= 0);
    }

    if (buf_size == 0) {
        return EOF;
    } else {
        ++ pointer;
        return buffer[pointer - 1];
    }
}

int
main(void)
{
    char int_buffer[INT_BUFFER_SIZE];
    size_t int_buffer_size = 0;

    int chr = get_char();

    long long int sum = 0;

    while (chr != EOF) {
        while (isspace(chr)) {
            chr = get_char();
        }

        while (chr != EOF && !isspace(chr)) {
            int_buffer[int_buffer_size] = chr;
            ++int_buffer_size;
            chr = get_char();
        }

        int_buffer [int_buffer_size] = 0;

        if (int_buffer_size > 0) {
            long long int value;
            sscanf(int_buffer, "%lld", &value);

            sum += value;
            int_buffer_size = 0;
        }
    }

    printf("%lld\n", sum);

    return 0;
}
