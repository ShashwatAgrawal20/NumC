#ifndef BASIC_OPS_H
#define BASIC_OPS_H

#include "ndarray.h"

/*
 * not sure if I want to use those nc_set and nc_get things??
 */
#define NC_BINARY_OP(type, op)                       \
    do {                                             \
        type *ad = (type *)a->data;                  \
        type *bd = (type *)b->data;                  \
        type *rd = (type *)result->data;             \
        for (size_t i = 0; i < a->total_size; ++i) { \
            rd[i] = ad[i] op bd[i];                  \
        }                                            \
    } while (0)

ndarray_t *nc_add(ndarray_t *a, ndarray_t *b);
ndarray_t *nc_sub(ndarray_t *a, ndarray_t *b);
ndarray_t *nc_mul(ndarray_t *a, ndarray_t *b);
ndarray_t *nc_div(ndarray_t *a, ndarray_t *b);

#endif  // !BASIC_OPS_H
