#include "numc/internal/broadcast.h"

#include <numc/pch.h>

#include "numc/core/ndarray.h"
#include "numc/internal/error_internal.h"

bool nc_can_broadcast(const ndarray_t *a, const ndarray_t *b) {
    if (!a || !b) {
        _nc_set_error(NC_ERR_NULL_INPUT);
        return false;
    }

    int result_ndim = (a->ndim > b->ndim) ? a->ndim : b->ndim;

    for (int i = 0; i < result_ndim; ++i) {
        int idx_a = a->ndim - 1 - i;
        int idx_b = b->ndim - 1 - i;

        size_t dim_a = (idx_a >= 0) ? a->shape[idx_a] : 1;
        size_t dim_b = (idx_b >= 0) ? b->shape[idx_b] : 1;

        if (dim_a != dim_b && dim_a != 1 && dim_b != 1) {
            return false;
        }
    }
    return true;
}
