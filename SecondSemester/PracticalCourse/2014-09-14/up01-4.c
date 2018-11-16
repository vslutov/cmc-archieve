#include <stdio.h>
#include <stdlib.h>

const double EPS = 1e-11;

int
compar(const void *pa, const void *pb)
{
    double a = *(double *) pa;
    double b = *(double *) pb;

    if (a < b - EPS) {
        return 1;
    } else if (b < a - EPS) {
        return -1;
    } else {
        return 0;
    }
}

int
main(void)
{
    double *unsorted_array = NULL;
    double *sorted_array = NULL;

    size_t num;
    scanf("%zu", &num);

    unsorted_array = calloc(num, sizeof(*unsorted_array));
    sorted_array = calloc(num, sizeof(*sorted_array));

    for (size_t i = 0; i < num; ++i) {
        scanf("%lf", &unsorted_array[i]);
        sorted_array[i] = unsorted_array[i];
    }

    qsort(sorted_array, num, sizeof(sorted_array[0]), compar);

    for (size_t i = 0; i < num; ++i) {
        printf("%zu %.10g %.10g\n", i + 1, unsorted_array[i], sorted_array[i]);
    }

    free(unsorted_array);
    free(sorted_array);

    return 0;
}

