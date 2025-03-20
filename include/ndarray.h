#ifndef NDARRAY_H
#define NDARRAY_H

#include <stdio.h>
#include <stdlib.h>

typedef enum { nc_int, nc_float, nc_double } dtype_t;

typedef struct {
    void *data;
    size_t *shape;
    size_t *strides;
    size_t total_size;
    size_t item_size;
    dtype_t dtype;
    int ndim;
} ndarray_t;

ndarray_t *nc_create(size_t *shape, int ndim, dtype_t dtype);
void nc_free(ndarray_t **array);

void nc_display(ndarray_t *array);
void *nc_get(ndarray_t *array, size_t *indices);
void nc_set(ndarray_t *array, size_t *indices, void *value);

#endif  // !NDARRAY_H
