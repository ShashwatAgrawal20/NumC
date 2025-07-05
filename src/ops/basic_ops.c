#include "numc/ops/basic_ops.h"

#include "numc/core/ndarray.h"
#include "numc/internal/broadcast.h"
#include "numc/internal/utils.h"

static inline bool _check_binary_op_compat(ndarray_t *a, ndarray_t *b,
                                           const char *op_name) {
    if (!a || !b) {
        _ELOG("%s error: one or both arrays are NULL\n", op_name);
        return false;
    }

    if (!nc_can_broadcast(a, b)) {
        _ELOG("%s error: dimension mismatch (%d vs %d)\n", op_name, a->ndim,
              b->ndim);
        return false;
    }

    if (a->dtype != b->dtype) {
        _ELOG("%s error: dtype mismatch (%d vs %d)\n", op_name, a->dtype,
              b->dtype);
        return false;
    }

    return true;
}

// Helper function to convert linear index to multi-dimensional indices
static inline void _linear_to_indices(size_t linear_idx, const size_t *shape,
                                      int ndim, size_t *indices) {
    for (int i = ndim - 1; i >= 0; i--) {
        indices[i] = linear_idx % shape[i];
        linear_idx /= shape[i];
    }
}

// Helper function to convert multi-dimensional indices to linear index
static inline size_t _indices_to_linear(const size_t *indices,
                                        const size_t *shape, int ndim) {
    size_t linear_idx = 0;
    size_t stride = 1;
    for (int i = ndim - 1; i >= 0; i--) {
        linear_idx += indices[i] * stride;
        stride *= shape[i];
    }
    return linear_idx;
}

// Helper function to broadcast indices from result shape to array shape
static inline size_t _broadcast_index(const size_t *result_indices,
                                      int result_ndim, const ndarray_t *arr) {
    size_t arr_indices[arr->ndim];

    // Map result indices to array indices (right-aligned)
    int offset = result_ndim - arr->ndim;
    for (int i = 0; i < arr->ndim; i++) {
        int result_dim = i + offset;
        if (result_dim >= 0) {
            // If array dimension is 1, broadcast (use index 0)
            // Otherwise use the corresponding result index
            arr_indices[i] =
                (arr->shape[i] == 1) ? 0 : result_indices[result_dim];
        } else {
            arr_indices[i] = 0;
        }
    }

    return _indices_to_linear(arr_indices, arr->shape, arr->ndim);
}

#define _NC_BINARY_OP_BROADCAST(type, op, safe_op)                            \
    do {                                                                      \
        type *ad = (type *)a->data;                                           \
        type *bd = (type *)b->data;                                           \
        type *rd = (type *)result->data;                                      \
        size_t result_indices[result_ndim];                                   \
        for (size_t i = 0; i < result->total_size; ++i) {                     \
            _linear_to_indices(i, result_shape, result_ndim, result_indices); \
            size_t a_idx = _broadcast_index(result_indices, result_ndim, a);  \
            size_t b_idx = _broadcast_index(result_indices, result_ndim, b);  \
            safe_op;                                                          \
            rd[i] = ad[a_idx] op bd[b_idx];                                   \
        }                                                                     \
    } while (0)

#define _CHECK_DIV_ZERO(a_idx, b_idx)                                    \
    if (bd[b_idx] == 0) {                                                \
        fprintf(stderr, "nc_div error: Division by zero at index %zu\n", \
                b_idx);                                                  \
        nc_free(&result);                                                \
        return NULL;                                                     \
    }

#define _NC_BINARY_OP_WRAPPER(operator, op_name, safe_op)                   \
    do {                                                                    \
        if (!_check_binary_op_compat(a, b, op_name)) {                      \
            return NULL;                                                    \
        }                                                                   \
        int result_ndim = (a->ndim > b->ndim) ? a->ndim : b->ndim;          \
        size_t result_shape[result_ndim];                                   \
        for (int i = 0; i < result_ndim; ++i) {                             \
            int idx_a = a->ndim - 1 - i;                                    \
            int idx_b = b->ndim - 1 - i;                                    \
            size_t dim_a = (idx_a >= 0) ? a->shape[idx_a] : 1;              \
            size_t dim_b = (idx_b >= 0) ? b->shape[idx_b] : 1;              \
            result_shape[result_ndim - 1 - i] =                             \
                (dim_a > dim_b) ? dim_a : dim_b;                            \
        }                                                                   \
        ndarray_t *result = nc_create(result_shape, result_ndim, a->dtype); \
        _check_null_return(result);                                         \
        switch (a->dtype) {                                                 \
            case nc_int:                                                    \
                _NC_BINARY_OP_BROADCAST(int, operator, safe_op);            \
                break;                                                      \
            case nc_float:                                                  \
                _NC_BINARY_OP_BROADCAST(float, operator, safe_op);          \
                break;                                                      \
            case nc_double:                                                 \
                _NC_BINARY_OP_BROADCAST(double, operator, safe_op);         \
                break;                                                      \
            default:                                                        \
                fprintf(stderr, "%s error: unsupported dtype\n", op_name);  \
                nc_free(&result);                                           \
                return NULL;                                                \
        }                                                                   \
        return result;                                                      \
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
    _NC_BINARY_OP_WRAPPER(/, "nc_div", _CHECK_DIV_ZERO(a_idx, b_idx));
}

#undef _CHECK_DIV_ZERO
#undef _NC_BINARY_OP_WRAPPER
#undef _NC_BINARY_OP_BROADCAST
