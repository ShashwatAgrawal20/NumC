#include "../include/ndarray_ops.h"

#include "../include/common.h"
#include "../include/ndarray.h"

ndarray_t *nc_add(ndarray_t *a, ndarray_t *b) {
    if (!a || !b) {
        fprintf(stderr, "nc_add error: one or both arrays are NULL\n");
        return NULL;
    }

    if (a->ndim != b->ndim) {
        fprintf(stderr, "nc_add error: dimension mismatch (%d vs %d)\n",
                a->ndim, b->ndim);
        return NULL;
    }

    for (int i = 0; i < a->ndim; ++i) {
        if (a->shape[i] != b->shape[i]) {
            fprintf(stderr,
                    "nc_add error: shape mismatch at axis %d (%zu vs %zu)\n", i,
                    a->shape[i], b->shape[i]);
            return NULL;
        }
    }

    ndarray_t *result = nc_create(a->shape, a->ndim, a->dtype);
    _check_null_return(result);

    switch (a->dtype) {
        case nc_int:
            NC_BINARY_OP(int, +);
            break;
        case nc_float:
            NC_BINARY_OP(float, +);
            break;
        case nc_double:
            NC_BINARY_OP(double, +);
            break;
        default:
            fprintf(stderr, "nc_add error: unsupported dtype\n");
            nc_free(&result);
            return NULL;
    }

    return result;
}
