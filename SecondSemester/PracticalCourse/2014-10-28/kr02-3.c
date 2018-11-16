#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

struct Data {
    double *data;
    unsigned short n;
};

struct Data *
read_data(int fd, int suffix)
{
    int pos = lseek(fd, suffix - sizeof(unsigned short), SEEK_END);
    if (pos < 0) {
        return NULL;
    }

    struct Data *result = calloc(1, sizeof(*result));
    read(fd, &(result->n), sizeof(result->n));

    result->data = calloc(result->n, sizeof(*(result->data)));
    lseek(fd, - sizeof(result->n) - result->n * sizeof(*(result->data)), SEEK_CUR);

    int r = result->n * sizeof(*(result->data));
    char *buffer = (char *) result->data;
    int a = 0;

    while(r > 0 && (a = read(fd, buffer, r)) > 0) {
        buffer += a;
        r -= a;
    }

    return result;
}

int
main (int argc, char **argv)
{
    int fd = open(argv[1], O_RDONLY, 0777);

    struct Data *data;
    int suffix = 0;

    while ((data = read_data(fd, suffix)) != NULL) {
        double avr = 0;
        if (data->n == 0) {
            printf("0\n");
        } else {
            for (int i = 0; i < data->n; ++i) {
                avr += log(data->data[i]);
            }
            avr = avr / data->n;
            avr = exp(avr);
            printf("%.10g\n", avr);
        }

        suffix -= sizeof(*(data->data)) * data->n + sizeof(data->n);
        free(data->data);
        free(data);
    }
}
