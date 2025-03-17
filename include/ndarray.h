#ifndef NDARRAY_H
#define NDARRAY_H

#include <stdio.h>
#include <stdlib.h>

typedef enum { INT32, FLOAT64 } dtype_t;

typedef struct {
    void *data;
    size_t *shape;
    size_t *strides;
    dtype_t dtype;
    int ndim;
} ndarray_t;

ndarray_t *nc_create(size_t *shape, int ndim, dtype_t dtype);
void nc_free(ndarray_t **array);

#endif  // !NDARRAY_H
