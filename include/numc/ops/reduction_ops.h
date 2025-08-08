#ifndef REDUCTION_OPS_H
#define REDUCTION_OPS_H
#include "numc/core/ndarray.h"

typedef struct {
    int axis;
    dtype_t dtype;
    ndarray_t *out;
    bool keepdims;
    int scalar;
    bool where;
} nc_sum_opts;

/*
 * WHAT IS NC_SUM_DEFAULT_OPTS?
 * ----------------------------
 * NC_SUM_DEFAULT_OPTS is a macro used to create and initialize a static
 * instance of `nc_sum_opts`, a configuration struct for the `nc_sum` reduction
 * operation. It provides a convenient way to use default settings and override
 * specific fields inline using GNU C extensions.
 *
 * This macro returns a pointer to a statically initialized `nc_sum_opts`
 * struct. It uses compound literals and designated initializers, making it
 * both flexible and expressive when configuring reduction options.
 *
 * EXAMPLES
 * --------
 * Explicit creation and later field assignment:
 *
 *     nc_sum_opts *opts = NC_SUM_DEFAULT_OPTS();
 *     opts->axis = 2;
 *     ndarray_t *result = nc_sum(array, opts);
 *
 * Inline override using designated initializers:
 *
 *     ndarray_t *result = nc_sum(array, NC_SUM_DEFAULT_OPTS(.axis = 2));
 *
 * DEFAULT VALUES
 * --------------
 * The following defaults are used when calling NC_SUM_DEFAULT_OPTS():
 *
 *     axis     = -1       // Reduce over all axes
 *     dtype    = -1       // Infer from input array
 *     out      = NULL     // Output is not preallocated
 *     keepdims = true     // Reduced dimensions retained as size 1
 *     scalar   = 0        // Not used (reserved for scalar input handling)
 *     where    = false    // Apply reduction unconditionally
 *
 * MACRO IMPLEMENTATION DETAILS
 * ----------------------------
 * This macro leverages GNU C extensions:
 * - Compound literals allow for on-the-fly struct instantiation.
 * - Designated initializers permit overriding specific struct fields.
 *
 * Because it returns a pointer to a static object, the returned pointer
 * should not be freed or modified concurrently from multiple threads.
 * Use with caution in multithreaded contexts or when long-lived mutations
 * are required.
 */
#define NC_SUM_DEFAULT_OPTS(...)      \
    (&(nc_sum_opts){.axis = -1,       \
                    .dtype = -1,      \
                    .out = NULL,      \
                    .keepdims = true, \
                    .scalar = 0,      \
                    .where = false,   \
                    __VA_ARGS__})

/**
 * nc_sum
 * ======
 * Reduces a NumC ndarray by summing along a given axis or all axes.
 *
 * SYNOPSIS
 * -------
 *     ndarray_t *nc_sum(const ndarray_t *array, const nc_sum_opts *opts);
 *
 * PARAMETERS
 * ---------
 * - `array` : input array to reduce.
 * - `opts`  : reduction options (see `nc_sum_opts`).
 *
 * RETURNS
 * -------
 * - New array with the reduction result, or `opts->out` if supported.
 * - NULL on failure.
 *
 * CURRENT SUPPORT
 * ---------------
 * Implemented:
 *    - axis  (incl. negative indexing, but -1 will give you issues
 *      because it's used for "reduce over all axes")
 *
 *   - dtype (only NC_USE_ARRAY_DTYPE case)
 *
 * Ignored:
 *   - out
 *   - keepdims
 *   - scalar
 *   - where
 *
 * NOTES
 * -----
 * Behavior may expand in future; see `NC_SUM_DEFAULT_OPTS` for defaults.
 */
ndarray_t *nc_sum(const ndarray_t *array, const nc_sum_opts *opts);

#endif  // !REDUCTION_OPS_H
