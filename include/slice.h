#ifndef SLICE_H
#define SLICE_H

#include <stddef.h>

#include "../include/ndarray.h"

typedef struct {
    size_t start;
    size_t end;
    size_t step;
} slice_t;

ndarray_t *nc_slice(ndarray_t *array, slice_t *slices, int n_slices);

#endif  // !SLICE_H
