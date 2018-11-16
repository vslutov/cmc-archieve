#include <stdbool.h>
#include <stdio.h>

#include <unistd.h>

struct MyFile
{
    int fd;
    int size;
    char *buf;

    const char *head;
    int real_size;
    bool eof;
};

int
maybe_read(struct MyFile *f)
{
    if (f->head < f->buf + f->real_size) {
        int result = *(const unsigned char *)(f->head);

        ++f->head;
        return result;
    } else {
        return EOF;
    }
}

int
mygetc(struct MyFile *f)
{
    if (f->head == NULL) {
        f->head = (const char *) f->buf;
        f->real_size = 0;
        f->eof = false;
    }

    if (f->eof == true) {
        return EOF;
    }

    int result = maybe_read(f);

    if (result != EOF) {
        return result;
    } else {
        f->real_size = read(f->fd, f->buf, f->size);
        f->head = (const char *)f->buf;

        result = maybe_read(f);
        if (result == EOF) {
            f->eof = true;
        }

        return result;
    }
}
