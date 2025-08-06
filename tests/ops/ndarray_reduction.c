#include <numc/numc.h>

#include "../../tec.h"

TEC(reduction, test_sum_with_axis) {
    ndarray_t *test =
        nc_reshape(nc_arange(1, 9, 1, nc_int), SND_INLINE(2, 2, 2), true);

    // Test sum along axis 2
    nc_sum_otps *opts = NC_SUM_DEFAULT_OPTS();
    opts->axis = 2;
    ndarray_t *sum_axis2 = nc_sum(test, opts);

    TEC_ASSERT(sum_axis2 != NULL);
    TEC_ASSERT_EQ(sum_axis2->ndim, 2);  // Reduced from 3D to 2D
    TEC_ASSERT_EQ(sum_axis2->shape[0], (size_t)2);
    TEC_ASSERT_EQ(sum_axis2->shape[1], (size_t)2);

    nc_free(&test);
    nc_free(&sum_axis2);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverride-init"
TEC(reduction, test_sum_variadic_syntax) {
    ndarray_t *test =
        nc_reshape(nc_arange(1, 9, 1, nc_int), SND_INLINE(2, 2, 2), true);

    // Test variadic syntax with axis=1 and dtype change
    ndarray_t *sum_variadic =
        nc_sum(test, NC_SUM_DEFAULT_OPTS(.axis = 1, .dtype = nc_double));

    TEC_ASSERT(sum_variadic != NULL);
    TEC_ASSERT_EQ(sum_variadic->dtype, (dtype_t)nc_double);
    TEC_ASSERT_EQ(sum_variadic->ndim, 2);  // Reduced from 3D to 2D

    nc_free(&test);
    nc_free(&sum_variadic);
}
#pragma GCC diagnostic pop
