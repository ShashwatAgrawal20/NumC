#ifndef NUMC_H
#define NUMC_H

#include "ndarray.h"

#define _check_alloc(allocation)                               \
    if (allocation == NULL) {                                  \
        fprintf(stderr, "Error: Memory allocation failed!\n"); \
        goto defer;                                            \
    }

#define _check_null_return(item) \
    if (item == NULL) {          \
        return 1;                \
    }

#define _check_zero(value) \
    if ((value) == 0) {    \
        return NULL;       \
    }

#endif  // !NUMC_H
