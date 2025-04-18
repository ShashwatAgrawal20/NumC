#ifndef NUMC_H
#define NUMC_H

#include "internal/core/ndarray.h"
#include "internal/core/slice.h"
#include "internal/ops/basic_ops.h"
#include "internal/ops/reduction_ops.h"

#define nc_dim_count(shape) (sizeof(shape) / sizeof((shape)[0]))
#define SND(shape) shape, nc_dim_count(shape)

ndarray_t *nc_create(size_t *shape, int ndim, dtype_t dtype);
void nc_free(ndarray_t **array);

ndarray_t *nc_arange(double start, double stop, double step, dtype_t dtype);
ndarray_t *nc_reshape(ndarray_t *array, size_t *shape, int ndim,
                      bool is_inline);

void nc_display(ndarray_t *array, bool print_data);

void *nc_get(ndarray_t *array, size_t *indices);
void nc_set(ndarray_t *array, size_t *indices, void *value);

ndarray_t *nc_slice(ndarray_t *array, slice_t *slices, int n_slices);


ndarray_t *nc_add(ndarray_t *a, ndarray_t *b);
ndarray_t *nc_sub(ndarray_t *a, ndarray_t *b);
ndarray_t *nc_mul(ndarray_t *a, ndarray_t *b);
ndarray_t *nc_div(ndarray_t *a, ndarray_t *b);


#endif  // !NUMC_H
