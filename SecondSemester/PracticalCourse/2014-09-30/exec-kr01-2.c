#include "kr01-2.c"

struct MyFile f;
char buf[64];

int
main(void)
{
    int c = 0;
    f.buf = buf;
    f.size = sizeof(buf);
    f.fd = 0;

    while ((c = mygetc(&f)) != EOF) {
        putchar(c);
    }

    return 0;
}
