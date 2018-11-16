#include "sum.h"

#include <stdlib.h>

float
sum(void *p_matrix, ssize_t size)
{
    typedef float Matrix[size][size];
    Matrix *matrix = p_matrix;
    float result = 0;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            result += (*matrix)[i][j];
        }
    }

    return result;
}

float
slow_sum(void *p_matrix, ssize_t size)
{
    typedef float Matrix[size][size];
    Matrix *matrix = p_matrix;
    float result = 0;

    for (int j = 0; j < size; j++) {
        for (int i = 0; i < size; i++) {
            result += (*matrix)[i][j];
        }
    }

    return result;
}
