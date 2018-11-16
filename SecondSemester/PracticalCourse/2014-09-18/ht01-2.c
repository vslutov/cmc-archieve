#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
    INIT_BUFFER_SIZE = 4,
    BUFFER_SIZE_MULTIPLIER = 2
};

char *
getline2(FILE *file)
{
    size_t real_size = INIT_BUFFER_SIZE;
    size_t virtual_size = 0;

    char *head = malloc(real_size * sizeof(*head));
    char *tail = head;

    do {
        if (virtual_size == real_size - 1) {
            real_size *= BUFFER_SIZE_MULTIPLIER;
            head = realloc(head, real_size * sizeof(*head));
            tail = head + virtual_size;
        }

        if (fgets(tail, real_size - (tail - head), file) == NULL) {
            if (virtual_size == 0) {
                free(head);
                return NULL;
            } else {
                break;
            }
        }

        virtual_size += strlen(tail);
    } while (head[virtual_size - 1] != '\n');

    head = realloc(head, virtual_size + 1);
    return head;
}

