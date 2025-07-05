#ifndef NDARRAY_H
#define NDARRAY_H

#include <numc/pch.h>

typedef enum { nc_int, nc_float, nc_double } dtype_t;

typedef struct {
    void *data;
    size_t *shape;
    size_t *strides;
    size_t total_size;
    size_t item_size;
    dtype_t dtype;
    int ndim;
    bool owns_data;
} ndarray_t;

ndarray_t *nc_create(size_t *shape, int ndim, dtype_t dtype);
void nc_free(ndarray_t **array);

ndarray_t *nc_arange(double start, double stop, double step, dtype_t dtype);

/**
 * @brief Reshapes an existing ndarray to a new shape, either in-place or by
 * creating a new array.
 *
 * @param array        Pointer to the original ndarray to reshape.
 * @param shape        Pointer to an array of shape dimensions.
 * @param ndim         Number of dimensions in the new shape.
 * @param is_inline    Whether to reshape the array in-place (`true`) or return
 * a new one (`false`).
 *
 * @return
 * - If `is_inline == true`: returns the same pointer as `array`, with updated
 * shape/strides.
 * - If `is_inline == false`: returns a new ndarray with copied data and the new
 * shape.
 * - Returns `NULL` on failure.
 *
 * @details
 * Use `is_inline = true` ONLY if:
 * - You're passing a temporary ndarray (e.g., `nc_arange(...)`) directly into
 * `nc_reshape(...)`, like:
 *   @code
 *   nc_reshape(nc_arange(0, 12, 1, nc_int), shape, 2, true);
 *   @endcode
 * - Or you're fully aware that this operation will **not** allocate a new
 * array, and the reshape will affect the original object in-place.
 *
 * @warning
 * When `is_inline` is true:
 * - The reshaped array is the SAME pointer as the original.
 * - Free **only one** of them (either the original or the reshaped).
 * - Do not assume the two are separate objects.
 *
 * @attention
 * 🚨 If you pass `is_inline = true` for an array stored in a variable (not a
 * temporary), and an error occurs during reshape, the array **may be freed**.
 *
 * 🚨 **DO NOT** use `is_inline = true` unless you're 100% sure you want the
 * original array to be modified and are okay with the risk of it being freed on
 * error.
 *
 * @todo
 * Consider factoring out internal shape/stride logic into a shared utility
 * to avoid duplication with `nc_create()`.
 */
ndarray_t *nc_reshape(ndarray_t *array, size_t *shape, int ndim,
                      bool is_inline);

/**
 * @brief Debug print for an ndarray_t struct.
 *
 * @param array Pointer to the ndarray.
 * @param print_data If true, prints data; else just metadata.
 */
void nc_display(ndarray_t *array, bool print_data);

void *nc_get(ndarray_t *array, size_t *indices);
void nc_set(ndarray_t *array, size_t *indices, void *value);

#endif  // !NDARRAY_H
