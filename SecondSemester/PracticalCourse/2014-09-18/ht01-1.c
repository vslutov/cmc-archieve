int
getmax2(int count, const int *array)
{
    int first_max, second_max;

    if (array[0] > array[1]) {
        first_max = array[0];
        second_max = array[1];
    } else {
        first_max = array[1];
        second_max = array[0];
    }

    for (int i = 2; i < count; ++i) {
        if (array[i] > first_max) {
            second_max = first_max;
            first_max = array[i];
        } else if (array[i] > second_max) {
            second_max = array[i];
        }
    }

    return second_max;
}

