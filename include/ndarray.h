#ifndef NDARRAY_H
#define NDARRAY_H

#include <stdbool.h>
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

void nc_display(ndarray_t *array, bool print_data);
void *nc_get(ndarray_t *array, size_t *indices);
void nc_set(ndarray_t *array, size_t *indices, void *value);
ndarray_t *nc_arange(double start, double stop, double step, dtype_t dtype);

/**
 * @brief FUCK OFF CLANGD
 * Notes on `is_inline`:
 * -----------------------------------------------------------------------------
 * Use `is_inline = true` ONLY when:
 *   * You're passing a temporary ndarray like `nc_arange(...)` directly into
 *     `nc_reshape(...)`, e.g.:
 *       nc_reshape(nc_arange(0, 12, 1, nc_int), shape, 2, true);
 *
 *   * Or you explicitly want to reshape in-place and understand that no new
 *     memory will be allocated.
 *
 * WARNING:
 * -----------------------------------------------------------------------------
 * When `is_inline` is true, the reshaped array is the SAME pointer as the
 * original. This means:
 *   - Free only ONE of them — either the reshaped result or the original.
 *   - If you store both pointers, do not assume they're separate objects.
 */
ndarray_t *nc_reshape(ndarray_t *array, size_t *shape, int ndim,
                      bool is_inline);

#endif  // !NDARRAY_H
