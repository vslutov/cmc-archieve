#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int
system(const char *s)
{
    int p, st;

    if((p = fork()) < 0) {
        return -1;
    } else if (!p) {
        execlp("/bin/sh", "/bin/sh", "-c", s, NULL);
        _exit(1);
    } else {
        waitpid(p, &st, 0);

        if (WIFEXITED(st) && WEXITSTATUS(st) == 0) {
            return 0;
        } else {
            return 1;
        }
    }
}

