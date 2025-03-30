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

#define _NC_BINARY_OP_WRAPPER(operator, op_name)                           \
    do {                                                                   \
        if (!_check_binary_op_compat(a, b, op_name)) {                     \
            return NULL;                                                   \
        }                                                                  \
        ndarray_t *result = nc_create(a->shape, a->ndim, a->dtype);        \
        _check_null_return(result);                                        \
        switch (a->dtype) {                                                \
            case nc_int:                                                   \
                NC_BINARY_OP(int, operator);                               \
                break;                                                     \
            case nc_float:                                                 \
                NC_BINARY_OP(float, operator);                             \
                break;                                                     \
            case nc_double:                                                \
                NC_BINARY_OP(double, operator);                            \
                break;                                                     \
            default:                                                       \
                fprintf(stderr, "%s error: unsupported dtype\n", op_name); \
                nc_free(&result);                                          \
                return NULL;                                               \
        }                                                                  \
        return result;                                                     \
    } while (0);

ndarray_t *nc_add(ndarray_t *a, ndarray_t *b) {
    _NC_BINARY_OP_WRAPPER(+, "nc_add");
}

#undef _NC_BINARY_OP_WRAPPER
