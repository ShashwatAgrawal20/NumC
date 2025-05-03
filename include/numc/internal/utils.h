#ifndef UTILS_H
#define UTILS_H

#include "numc/core/ndarray.h"

#define NC_USE_ARRAY_DTYPE ((dtype_t) - 1)
#define _check_fail() goto defer

#define _check_alloc(allocation)                                      \
    if (allocation == NULL) {                                         \
        fprintf(stderr, "malloc error: Memory allocation failed!\n"); \
        goto defer;                                                   \
    }

#define _check_null_return(item) \
    if (item == NULL) {          \
        return NULL;             \
    }

#define _check_zero(value) \
    if ((value) == 0) {    \
        return NULL;       \
    }

#define _ELOG(...)                    \
    do {                              \
        fprintf(stderr, __VA_ARGS__); \
    } while (0)

#define _GUARD(expr, ...)                 \
    do {                                  \
        if (expr) {                       \
            fprintf(stderr, __VA_ARGS__); \
            return NULL;                  \
        }                                 \
    } while (0)

static inline void _assign_value(void *ptr, double val, dtype_t dtype) {
    switch (dtype) {
        case nc_int:
            *(int *)ptr = (int)val;
            break;
        case nc_float:
            *(float *)ptr = (float)val;
            break;
        case nc_double:
            *(double *)ptr = val;
            break;
        default:
            _ELOG("_assign_value error: invalid dtype (%d)\n", dtype);
    }
}

static inline size_t _dtype_size(dtype_t dtype) {
    switch (dtype) {
        case nc_int:
            return sizeof(int);
        case nc_float:
            return sizeof(float);
        case nc_double:
            return sizeof(double);
        default:
            _ELOG("_dtype_size error: invalid dtype (%d)\n", dtype);
            return 0;
    }
}

#endif  // !UTILS_H
