#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main(void)
{
    setbuf(stdout, NULL);
    int fd[2];
    pipe(fd);

    if (fork()) {
        close(fd[0]);
        time_t current_time = time(NULL);

        write(fd[1], &current_time, sizeof(current_time));
        write(fd[1], &current_time, sizeof(current_time));
        write(fd[1], &current_time, sizeof(current_time));
        close(fd[1]);

        wait(NULL);
    } else if (fork()) {
        close(fd[1]);

        time_t current_time;
        read(fd[0], &current_time, sizeof(current_time));
        close(fd[0]);

        struct tm *str_time = localtime(&current_time);

        wait(NULL);

        printf("H:%02d\n", str_time->tm_hour);
    } else if (fork()) {
        close(fd[1]);

        time_t current_time;
        read(fd[0], &current_time, sizeof(current_time));
        close(fd[0]);

        struct tm *str_time = localtime(&current_time);

        wait(NULL);

        printf("M:%02d\n", str_time->tm_min);
    } else {
        close(fd[1]);

        time_t current_time;
        read(fd[0], &current_time, sizeof(current_time));
        close(fd[0]);

        struct tm *str_time = localtime(&current_time);
        printf("S:%02d\n", str_time->tm_sec);
    }

    return 0;
}

