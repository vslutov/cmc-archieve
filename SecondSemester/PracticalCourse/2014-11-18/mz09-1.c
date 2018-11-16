#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <fcntl.h>
#include <stdlib.h>

int main(void)
{
    int fd, i, r;
    short s;
    fd = open("input.txt", O_WRONLY | O_TRUNC | O_CREAT, 0600);
    write(fd, "12345678", 8);
    close(fd);
    fd = open("input.txt", O_RDONLY);
    if (!fork()) {
        for (i = 0; i < 1; ++i) {
            if (read(fd, &s, sizeof(s)) == 2) {
                write(1, &s, sizeof(s));
            }
        }
    }
    usleep(1);
    usleep(1);
    lseek(fd, 4, SEEK_CUR);
    for (i = 0; i < 1; ++i) {
        r = read(fd, &s, sizeof(s));
        wait(NULL);
        if (r == 2) {
            write(1, &s, sizeof(s));
        }
        lseek(fd, -4, SEEK_CUR);
    }
    return 0;
}

