#ifndef BASIC_OPS_H
#define BASIC_OPS_H

#include "../core/ndarray.h"

ndarray_t *nc_add(ndarray_t *a, ndarray_t *b);
ndarray_t *nc_sub(ndarray_t *a, ndarray_t *b);
ndarray_t *nc_mul(ndarray_t *a, ndarray_t *b);
ndarray_t *nc_div(ndarray_t *a, ndarray_t *b);

#endif  // !BASIC_OPS_H
