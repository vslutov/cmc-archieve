#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

int
main(int argc, char **argv)
{
    int count;
    sscanf(argv[1], "%d", &count);

    int file = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0777);

    int a0;
    sscanf(argv[3], "%d", &a0);

    int d;
    sscanf(argv[4], "%d", d);

    int k;
    sscanf(argv[5], "%d", &k);

    for (int i = 1; i < count; ++i) {
        if (fork() == 0) {

            for (int j = 0; j < k; ++ j) {
                int index = i + j * count;
                int a = a0 + index * d;

                lseek(file, index * sizeof(a), SEEK_SET);
                write(file, &a, sizeof(a));
            }

            return 0;
        }
    }

    for (int i = 0; i < count; ++i) {
        wait(NULL);
    }

    return 0;
}

