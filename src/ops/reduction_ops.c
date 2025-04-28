#include "numc/ops/reduction_ops.h"

#include <string.h>

#include "numc/core/ndarray.h"
#include "numc/internal/utils.h"
#include "numc/utils/macros.h"

ndarray_t *nc_sum(ndarray_t *array, int axis) {
    _GUARD((!array), "nc_sum error: invalid array input");

    int effective_axis = axis;
    if (axis < 0 && axis != -1) {
        effective_axis = array->ndim + axis;
    }

    _GUARD(
        (axis != -1 && (effective_axis < 0 || effective_axis >= array->ndim)),
        "nc_sum error: axis %d out of bounds for ndim %d\n", axis, array->ndim);

    if (axis == -1) {
        double acc = 0.0;

#define INTERNAL_NC_SUM_ALL(DTYPE)                   \
    for (size_t i = 0; i < array->total_size; ++i) { \
        acc += ((DTYPE *)array->data)[i];            \
    }

        switch (array->dtype) {
            case nc_int:
                INTERNAL_NC_SUM_ALL(int);
                break;
            case nc_float:
                INTERNAL_NC_SUM_ALL(float);
                break;
            case nc_double:
                INTERNAL_NC_SUM_ALL(double);
                break;
        }
#undef INTERNAL_NC_SUM_ALL

        ndarray_t *result = nc_create(SND_INLINE(1), array->dtype);
        _GUARD(!result, "nc_sum error: cannot create output array\n");

        _assign_value(result->data, acc, array->dtype);
        return result;
    }

    size_t new_shape[array->ndim - 1];
    int new_dim_idx = 0;
    for (int i = 0; i < array->ndim; ++i) {
        if (i != effective_axis) {
            new_shape[new_dim_idx++] = array->shape[i];
        }
    }
    ndarray_t *result = nc_create(SND(new_shape), array->dtype);
    _GUARD(!result, "nc_sum error: cannot create output array\n");

    size_t outer_dims_size = 1;
    size_t inner_dims_size = 1;
    for (int i = 0; i < array->ndim; i++) {
        if (i < effective_axis) {
            outer_dims_size *= array->shape[i];
        } else if (i > effective_axis) {
            inner_dims_size *= array->shape[i];
        }
    }

    size_t axis_size = array->shape[effective_axis];

#define SUM_ALONG_AXIS(DTYPE)                                          \
    do {                                                               \
        DTYPE *src_data = (DTYPE *)array->data;                        \
        DTYPE *dst_data = (DTYPE *)result->data;                       \
                                                                       \
        for (size_t i = 0; i < outer_dims_size; ++i) {                 \
            for (size_t j = 0; j < inner_dims_size; ++j) {             \
                size_t dst_idx = i * inner_dims_size + j;              \
                                                                       \
                for (size_t k = 0; k < axis_size; ++k) {               \
                    size_t src_idx = i * axis_size * inner_dims_size + \
                                     k * inner_dims_size + j;          \
                    dst_data[dst_idx] += src_data[src_idx];            \
                }                                                      \
            }                                                          \
        }                                                              \
    } while (0)

    switch (array->dtype) {
        case nc_int:
            SUM_ALONG_AXIS(int);
            break;
        case nc_float:
            SUM_ALONG_AXIS(float);
            break;
        case nc_double:
            SUM_ALONG_AXIS(double);
            break;
    }
#undef SUM_ALONG_AXIS
    return result;
}
