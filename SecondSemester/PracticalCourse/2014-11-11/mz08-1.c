#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    printf("1");
    setbuf(stdout, NULL);
    if (!fork()) {
        printf("2");
    }
    wait(NULL);
    printf("3");
    return 0;
}

