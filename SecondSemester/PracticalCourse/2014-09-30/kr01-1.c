#include <inttypes.h>
#include <stdio.h>
#include <string.h>

int
main(int argc, char **argv)
{
    intmax_t sum = 0;

    float average = 0;
    size_t count = 0;

    size_t length = 0;
    
    for (size_t i = 1; i < argc; ++i) {
        if (argv[i][0] == 'i') {
            intmax_t val;
            sscanf(argv[i] + 1, "%jd", &val);
            sum += val;
        } else if (argv[i][0] == 'd') {
            float val;
            sscanf(argv[i] + 1, "%g", &val);
            average += val;
            ++count;
        } else {
            length += strlen(argv[i]);
        }
    }

    if (count != 0) {
        average /= count;
    }

    printf(
        "%.10g\n%jd\n%zu\n",
        average,
        sum,
        length);
}
