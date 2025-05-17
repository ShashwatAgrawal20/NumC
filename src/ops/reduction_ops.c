#include "numc/ops/reduction_ops.h"

#include <string.h>

#include "numc/core/ndarray.h"
#include "numc/internal/utils.h"
#include "numc/utils/macros.h"

ndarray_t *nc_sum(const ndarray_t *array, const nc_sum_otps *opts) {
    _GUARD((!array), "nc_sum error: invalid array input");

    nc_sum_otps local_opts = *opts;
    if (local_opts.dtype == NC_USE_ARRAY_DTYPE) {
        local_opts.dtype = array->dtype;
    }

    int effective_axis = local_opts.axis;
    if (local_opts.axis < 0 && opts->axis != -1) {
        effective_axis = array->ndim + local_opts.axis;
    }

    _GUARD((local_opts.axis != -1 &&
            (effective_axis < 0 || effective_axis >= array->ndim)),
           "nc_sum error: axis %d out of bounds for ndim %d\n", local_opts.axis,
           array->ndim);

    if (local_opts.axis == -1) {
        double acc = 0.0;

#define INTERNAL_NC_SUM_ALL(DTYPE, _)                                          \
    do {                                                                       \
        size_t hehe = 0;                                                       \
        size_t limit = array->total_size - (array->total_size % 4);            \
        DTYPE *data = (DTYPE *)array->data;                                    \
        for (; hehe < limit; hehe += 4) {                                      \
            acc +=                                                             \
                data[hehe] + data[hehe + 1] + data[hehe + 2] + data[hehe + 3]; \
        }                                                                      \
        for (; hehe < array->total_size; ++hehe) {                             \
            acc += data[hehe];                                                 \
        }                                                                      \
    } while (0)

        ndarray_t *result = nc_create(SND_INLINE(1), local_opts.dtype);
        _GUARD(!result, "nc_sum error: cannot create output array\n");

        DISPATCH_DTYPE_MACRO(INTERNAL_NC_SUM_ALL, NULL);
        _assign_value(result->data, acc, local_opts.dtype);
        return result;
    }

    size_t new_shape[array->ndim - 1];
    int new_dim_idx = 0;
    for (int i = 0; i < array->ndim; ++i) {
        if (i != effective_axis) {
            new_shape[new_dim_idx++] = array->shape[i];
        }
    }
    ndarray_t *result = nc_create(SND(new_shape), local_opts.dtype);
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

#define SUM_ALONG_AXIS(DTYPE, DST_DTYPE)                               \
    do {                                                               \
        DTYPE *src_data = (DTYPE *)array->data;                        \
        DST_DTYPE *dst_data = (DST_DTYPE *)result->data;               \
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
    } while (0);

    switch (local_opts.dtype) {
        case nc_int:
            DISPATCH_DTYPE_MACRO(SUM_ALONG_AXIS, ;, int);
            break;
        case nc_float:
            DISPATCH_DTYPE_MACRO(SUM_ALONG_AXIS, ;, float);
            break;
        case nc_double:
            DISPATCH_DTYPE_MACRO(SUM_ALONG_AXIS, ;, double);
            break;
    }

#undef SUM_ALONG_AXIS
#undef INTERNAL_NC_SUM_ALL
    return result;
}
