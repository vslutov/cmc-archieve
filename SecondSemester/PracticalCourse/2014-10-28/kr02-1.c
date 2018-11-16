#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int
main(int argc, char **argv)
{
    for (int i = 1; i < argc; ++i) {
        int pid = fork();

        if (pid == -1) {
            return 1;
        } else if (pid == 0) {
            execlp(argv[i], argv[i], NULL);
            return 1;
        } else {
            int status;
            wait(&status);

            if (!WIFEXITED(status) || !(WEXITSTATUS(status) == 0)) {
                return 1;
            }
        }
    }

    return 0;
}

