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
    char *file1 = argv[4];
    char *file2 = argv[5];
    int fd[2];
    pipe(fd);

    if (!fork()) {
        close(fd[0]);
        dup2(fd[1], 1);
        close(fd[1]);

        if (!fork()) {
            int fdr = open(file1, O_RDONLY, 0777);
            dup2(fdr, 0);
            close(fdr);

            execlp(cmd1, cmd1, NULL);
            return 1;
        } else {
            int status;
            wait(&status);
            if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                execlp(cmd2, cmd2, NULL);
            }
            return 1;
        }
    } else {
        close(fd[1]);
        if (!fork()) {
            dup2(fd[0], 0);
            close(fd[0]);

            int fdw = open(file2, O_WRONLY | O_CREAT | O_APPEND, 0777);
            dup2(fdw, 1);
            close(fdw);

            execlp(cmd3, cmd3, NULL);
            return 1;
        } else {
            wait(NULL);
            wait(NULL);
            return 0;
        }
    }
}

