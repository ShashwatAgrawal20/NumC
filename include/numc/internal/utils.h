#ifndef UTILS_H
#define UTILS_H

#include "numc/core/ndarray.h"
#include "numc/internal/error_internal.h"

#define NC_USE_ARRAY_DTYPE ((dtype_t) - 1)
#define _check_fail() goto defer

#define _check_alloc(allocation)         \
    if (allocation == NULL) {            \
        _nc_set_error(NC_ERR_MEM_ALLOC); \
        goto defer;                      \
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

#define _GUARD(expr, error_code)       \
    do {                               \
        if (expr) {                    \
            _nc_set_error(error_code); \
            return NULL;               \
        }                              \
    } while (0)

#define DISPATCH_DTYPE_MACRO(MACRO, OPT_BODY, ...) \
    do {                                           \
        switch (array->dtype) {                    \
            case nc_int:                           \
                OPT_BODY;                          \
                MACRO(int, __VA_ARGS__);           \
                break;                             \
            case nc_float:                         \
                OPT_BODY;                          \
                MACRO(float, __VA_ARGS__);         \
                break;                             \
            case nc_double:                        \
                OPT_BODY;                          \
                MACRO(double, __VA_ARGS__);        \
                break;                             \
        }                                          \
    } while (0);

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
            _nc_set_error(NC_ERR_UNSUPPORTED_DTYPE);
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
            _nc_set_error(NC_ERR_UNSUPPORTED_DTYPE);
            return 0;
    }
}

#endif  // !UTILS_H
