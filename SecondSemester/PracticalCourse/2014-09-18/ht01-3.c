#include <math.h>

#include <stdbool.h>
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

int
main(void)
{
    char *string;

    while ((string = getline2(stdin)) != NULL) {
        size_t count = 0;

        double sum = 0;
        double elem;
        size_t just_readed;
        bool have_int = false;

        while (sscanf(string + count, "%lg%n", &elem, &just_readed) > 0) {
            count += just_readed;
            sum += elem;
            have_int = true;
        }

        if (have_int == false) {
            sum = NAN;
        }

        printf("%.10lg\n", sum);
        free(string);
    }

    return 0;
}

