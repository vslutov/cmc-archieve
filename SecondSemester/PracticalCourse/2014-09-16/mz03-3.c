#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>

enum {
    MAX_LEN = 16
};

struct Node
{
    int key;
    int left_idx;
    int right_idx;
};

struct Node
get_node(int fd, int index)
{
    struct Node result;

    char *buffer = (char *) &result;
    int size = sizeof(result);

    lseek(fd, index * size, SEEK_SET);

    int r;

    while (size > 0 && (r = read(fd, buffer, size)) > 0) {
        size -= r;
        buffer += r;
    }

    return result;
}

void
dfs(int fd, int index)
{
    struct Node current = get_node(fd, index);

    if (current.right_idx > 0) {
        dfs(fd, current.right_idx);
    }

    char string[MAX_LEN];
    sprintf(string, "%d\n", current.key);
    int size = strlen(string);

    int w = 0;
    char *buffer = string;

    while (size > 0 && (w = write(1, buffer, size)) > 0) {
        size -= w;
        buffer += w;
    }

    if (current.left_idx > 0) {
        dfs(fd, current.left_idx);
    }
}

int
main(int argc, char **argv)
{
    int fdr = open(argv[1], O_RDONLY, 0);

    dfs(fdr, 0);
    if (write(1, "\n", 1) != 1) {
        int garbage;
        static const char error_message[] = "Oh My God!\nCritical error.\n"
            "Program cannot continue execition.\n";
        garbage = write(2, error_message, sizeof(error_message));
        garbage = garbage;

        exit(1);
    }

    return 0;
}

