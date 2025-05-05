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
} nc_sum_otps;

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
    (&(nc_sum_otps){.axis = -1,       \
                    .dtype = -1,      \
                    .out = NULL,      \
                    .keepdims = true, \
                    .scalar = 0,      \
                    .where = false,   \
                    __VA_ARGS__})

/**
 * nc_sum
 * ======
 * Performs a summation (reduction) operation over one or more axes of a NumC
 * ndarray.
 *
 * SYNOPSIS
 * --------
 *     ndarray_t *nc_sum(const ndarray_t *array, const nc_sum_otps *opts);
 *
 * DESCRIPTION
 * -----------
 * The `nc_sum` function reduces the input `array` by summing its elements
 * across the specified axis or axes. The behavior and output of the reduction
 * can be controlled through the `nc_sum_otps` options struct, allowing
 * fine-grained configuration including output data type, whether to retain
 * reduced dimensions, and whether to reduce conditionally.
 *
 * PARAMETERS
 * ----------
 * - `array` : `const ndarray_t *`
 *     A pointer to the input NumC ndarray to be reduced.
 *
 * - `opts` : `const nc_sum_otps *`
 *     A pointer to an options struct that controls reduction behavior. This can
 * be constructed using the `NC_SUM_DEFAULT_OPTS(...)` macro for convenience.
 *
 * OPTIONS STRUCTURE: `nc_sum_otps`
 * -------------------------------
 * - `axis` : `int`
 *     The axis along which to perform the summation. A value of `-1` means
 * reduce over all axes.
 *
 * - `dtype` : `dtype_t`
 *     Specifies the data type of the result. A value of `-1` indicates the type
 * should be inferred from the input array.
 *
 * - `out` : `ndarray_t *`
 *     Optional pre-allocated output array. If NULL, a new array is allocated
 * and returned.
 *
 * - `keepdims` : `bool`
 *     If `true`, retains reduced axes as dimensions of size 1, preserving the
 * input shape's rank.
 *
 * - `scalar` : `int`
 *     Reserved for scalar reduction logic. Not currently used.
 *
 * - `where` : `bool`
 *     If `true`, a masking mechanism is applied to select which elements are
 * included in the reduction. (Implementation dependent or reserved for future
 * support.)
 *
 * RETURNS
 * -------
 * - Returns a newly allocated `ndarray_t *` containing the result of the
 * reduction unless an output array was provided via `opts->out`, in which case
 * that array is returned.
 * - Returns `NULL` if the operation fails (e.g., due to incompatible shapes or
 * memory issues).
 *
 * EXAMPLES
 * --------
 * Reduce across all axes:
 *     ndarray_t *sum = nc_sum(array, NC_SUM_DEFAULT_OPTS());
 *
 * Reduce along axis 0 and retain dimensions:
 *     ndarray_t *sum = nc_sum(array, NC_SUM_DEFAULT_OPTS(.axis = 0, .keepdims =
 * true));
 *
 * Reduce to pre-allocated output array:
 *     ndarray_t *sum = nc_sum(array, NC_SUM_DEFAULT_OPTS(.axis = 1, .out =
 * prealloc));
 *
 * NOTES
 * -----
 * - This function assumes valid and well-formed inputs; no internal validation
 * is guaranteed.
 * - Returned arrays must be manually freed unless passed in via `opts->out`.
 * - Behavior with `scalar` and `where` fields may be subject to future
 * expansion.
 *
 * SEE ALSO
 * --------
 * - `NC_SUM_DEFAULT_OPTS` macro for convenient default options.
 * - `ndarray_t` struct definition and utility functions.
 */
ndarray_t *nc_sum(const ndarray_t *array, const nc_sum_otps *opts);

#endif  // !REDUCTION_OPS_H
