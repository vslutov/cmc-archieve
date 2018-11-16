#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <fcntl.h>

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    setbuf(stdout, NULL);
    char *file1 = argv[1];
    intmax_t val2;
    sscanf(argv[2], "%jd", &val2);
    intmax_t val3;
    sscanf(argv[3], "%jd", &val3);


    int fd[2];
    pipe(fd);

    if (!fork()) {
        close(fd[1]);

        intmax_t sum = 0;
        int32_t movefrom;

        int pid;
        read(fd[0], &pid, sizeof(pid));

        while (read(fd[0], &movefrom, sizeof(movefrom)) == sizeof(movefrom)) {
            sum += movefrom;
            if (sum > val3) {
                printf("-1\n");
                kill(pid, SIGTERM);
                return 0;
            }
        }
        printf("%jd\n", sum);
    } else if (!fork()) {
        close(fd[0]);
        int fdr = open(file1, O_RDONLY, 0777);
        dup2(fdr, 0);
        close(fdr);
        if (!fork()) {
            intmax_t val;
            int32_t moveto;
            int pid = getpid();
            write(fd[1], &pid, sizeof(pid));
            while (scanf("%jd", &val) > 0) {
                moveto = val * val % val2;
                write(fd[1], &moveto, sizeof(moveto));
            }
        } else {
            wait(NULL);
        }
    } else {
        close(fd[0]);
        close(fd[1]);
        wait(NULL);
        wait(NULL);
        printf("0\n");
    }

    return 0;
}

