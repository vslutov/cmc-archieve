#include <inttypes.h>

int
sys_write(int fd, const void *buf, int size);

const char hello[] = "Hello, World!\n";

int
main(void)
{
    sys_write(1, hello, sizeof(hello) - 1);
    return 0;
}
