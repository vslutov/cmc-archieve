#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct FileList {
    FILE *file;
    long long int head;
    bool open;
};

int
main(int argc, char **argv)
{
    size_t file_count = argc - 1;
    struct FileList *array = calloc(file_count, sizeof(*array));

    size_t opened_files = 0;

    for (size_t i = 0; i < file_count; ++i) {
        array[i].file = fopen(argv[i + 1], "r");
        if (fscanf(array[i].file, "%lld", &array[i].head) > 0) {
            array[i].open = true;
            ++opened_files;
        } else {
            array[i].open = false;
            fclose(array[i].file);
        }
    }

    while (opened_files > 0) {
        size_t min = 0;
        bool have_opened_file = false;

        for (size_t i = 0; i < file_count; ++i) {
            if (
                array[i].open &&
                (have_opened_file == false || array[i].head < array[min].head)
                ) {

                min = i;
                have_opened_file = true;

            }
        }

        printf("%lld ", array[min].head);

        if (fscanf(array[min].file, "%lld", &array[min].head) <= 0) {
            array[min].open = false;
            fclose(array[min].file);
            --opened_files;
        }
    }

    free(array);
    printf("\n");

    return 0;
}

