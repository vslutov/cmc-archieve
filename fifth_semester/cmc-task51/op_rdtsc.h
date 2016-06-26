#ifndef OP_RDTSC_H
#define OP_RDTSC_H

#include "ElproConfig.h"
#include <inttypes.h>

#ifdef HAVE_RDTSC
uint64_t
op_rdtsc(void);
#endif

#endif // OP_RDTSC_H
