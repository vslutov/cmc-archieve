#ifndef PROFILER_H
#define PROFILER_H

#include <stdlib.h>

typedef void VoidFunction(void);

char *
profile(VoidFunction, ssize_t);

#endif
