#include "../../include/basic_ops.h"

#include "../../include/ndarray.h"
#include "../../include/utils.h"

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

#define _NC_BINARY_OP(type, op, safe_op)             \
    do {                                             \
        type *ad = (type *)a->data;                  \
        type *bd = (type *)b->data;                  \
        type *rd = (type *)result->data;             \
        for (size_t i = 0; i < a->total_size; ++i) { \
            safe_op;                                 \
            rd[i] = ad[i] op bd[i];                  \
        }                                            \
    } while (0)

#define _CHECK_DIV_ZERO(i)                                                   \
    if (bd[i] == 0) {                                                        \
        fprintf(stderr, "nc_div error: Division by zero at index %zu\n", i); \
        nc_free(&result);                                                    \
        return NULL;                                                         \
    }

#define _NC_BINARY_OP_WRAPPER(operator, op_name, safe_op)                  \
    do {                                                                   \
        if (!_check_binary_op_compat(a, b, op_name)) {                     \
            return NULL;                                                   \
        }                                                                  \
        ndarray_t *result = nc_create(a->shape, a->ndim, a->dtype);        \
        _check_null_return(result);                                        \
        switch (a->dtype) {                                                \
            case nc_int:                                                   \
                _NC_BINARY_OP(int, operator, safe_op);                     \
                break;                                                     \
            case nc_float:                                                 \
                _NC_BINARY_OP(float, operator, safe_op);                   \
                break;                                                     \
            case nc_double:                                                \
                _NC_BINARY_OP(double, operator, safe_op);                  \
                break;                                                     \
            default:                                                       \
                fprintf(stderr, "%s error: unsupported dtype\n", op_name); \
                nc_free(&result);                                          \
                return NULL;                                               \
        }                                                                  \
        return result;                                                     \
    } while (0);

ndarray_t *nc_add(ndarray_t *a, ndarray_t *b) {
    _NC_BINARY_OP_WRAPPER(+, "nc_add", (void)0);
}

ndarray_t *nc_mul(ndarray_t *a, ndarray_t *b) {
    _NC_BINARY_OP_WRAPPER(*, "nc_mul", (void)0);
}

ndarray_t *nc_sub(ndarray_t *a, ndarray_t *b) {
    _NC_BINARY_OP_WRAPPER(-, "nc_sub", (void)0);
}

ndarray_t *nc_div(ndarray_t *a, ndarray_t *b) {
    _NC_BINARY_OP_WRAPPER(/, "nc_div", _CHECK_DIV_ZERO(i));
}

#undef _CHECK_DIV_ZERO
#undef _NC_BINARY_OP_WRAPPER
#undef _NC_BINARY_OP
