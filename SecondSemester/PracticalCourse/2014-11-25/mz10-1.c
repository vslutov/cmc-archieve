#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    setbuf(stdout, NULL);
    char *cmd1 = argv[1];
    char *cmd2 = argv[2];
    char *cmd3 = argv[3];

    if (!fork()) {
        dup2(1, 2);
        if (!fork()) {
            execlp(cmd1, cmd1, NULL);
            return 1;
        } else {
            int status;
            wait(&status);
            status = WIFEXITED(status) && WEXITSTATUS(status) == 0 ? 0 : 1;

            if (status == 1) {
                execlp(cmd2, cmd2, NULL);
            }
            return 1;
        }
    } else {
        int status;
        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            if (!fork()) {
                execlp(cmd3, cmd3, NULL);
                return 1;
            } else {
                wait(&status);
                status = WIFEXITED(status) && WEXITSTATUS(status) == 0 ? 0 : 1;
                return status;
            }
        } else {
            return 1;
        }
    }
}

