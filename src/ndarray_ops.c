#include "../include/ndarray_ops.h"

#include "../include/ndarray.h"
#include "../include/utils.h"

static inline bool _check_binary_op_compat(ndarray_t *a, ndarray_t *b,
                                           const char *op_name) {
    if (!a || !b) {
        fprintf(stderr, "%s error: one or both arrays are NULL\n", op_name);
        return false;
    }

    if (a->ndim != b->ndim) {
        fprintf(stderr, "%s error: dimension mismatch (%d vs %d)\n", op_name,
                a->ndim, b->ndim);
        return false;
    }

    if (a->dtype != b->dtype) {
        fprintf(stderr, "%s error: dtype mismatch (%d vs %d)\n", op_name,
                a->dtype, b->dtype);
        return false;
    }

    for (int i = 0; i < a->ndim; ++i) {
        if (a->shape[i] != b->shape[i]) {
            fprintf(stderr,
                    "%s error: shape mismatch at axis %d (%zu vs %zu)\n",
                    op_name, i, a->shape[i], b->shape[i]);
            return false;
        }
    }

    return true;
}

ndarray_t *nc_add(ndarray_t *a, ndarray_t *b) {
    if (!_check_binary_op_compat(a, b, "nc_add")) {
        return NULL;
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
