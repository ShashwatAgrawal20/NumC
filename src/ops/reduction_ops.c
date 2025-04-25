#include "numc/ops/reduction_ops.h"

#include "numc/core/ndarray.h"
#include "numc/internal/utils.h"

ndarray_t *nc_sum(ndarray_t *array, int axis) {
    _GUARD((!array), "nc_sum error: invalid array input");
    _GUARD(axis < -1 || axis >= array->ndim,
           "nc_sum error: axis %d out of bounds for ndim %d\n", axis,
           array->ndim);

#define INTERNAL_NC_SUM(DTYPE)                           \
    do {                                                 \
        for (size_t i = 0; i < array->total_size; ++i) { \
            acc += ((DTYPE *)array->data)[i];            \
        }                                                \
    } while (0);

    if (axis == -1) {
        double acc = 0.0;

        switch (array->dtype) {
            case nc_int:
                INTERNAL_NC_SUM(int);
                break;
            case nc_float:
                INTERNAL_NC_SUM(float);
                break;
            case nc_double:
                INTERNAL_NC_SUM(double);
                break;
        }
        size_t shape[1] = {1};
        ndarray_t *result = nc_create(shape, 1, array->dtype);
        _GUARD(!result, "nc_sum error: cannot create a output array\n");
        _assign_value(result->data, acc, array->dtype);
        return result;
    }
    return NULL;
#undef INTERNAL_NC_SUM
}
