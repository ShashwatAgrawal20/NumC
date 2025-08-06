#include <numc/numc.h>

#include "../../tec.h"

TEC(reshape, test_reshape_basic) {
    ndarray_t *original = nc_arange(0, 12, 1, nc_int);
    size_t shape[] = {3, 4};
    ndarray_t *reshaped = nc_reshape(original, shape, 2, true);

    TEC_ASSERT(reshaped != NULL);
    TEC_ASSERT_EQ(reshaped->ndim, 2);
    TEC_ASSERT_EQ(reshaped->shape[0], (size_t)3);
    TEC_ASSERT_EQ(reshaped->shape[1], (size_t)4);
    TEC_ASSERT_EQ(reshaped->total_size, (size_t)12);

    int *data = (int *)reshaped->data;
    TEC_ASSERT_EQ(data[0], 0);
    TEC_ASSERT_EQ(data[11], 11);

    nc_free(&reshaped);
}

TEC(reshape, test_reshape_inline_macro) {
    ndarray_t *reshaped =
        nc_reshape(nc_arange(0, 12, 1, nc_int), SND_INLINE(3, 4), true);

    TEC_ASSERT(reshaped != NULL);
    TEC_ASSERT_EQ(reshaped->ndim, 2);
    TEC_ASSERT_EQ(reshaped->shape[0], (size_t)3);
    TEC_ASSERT_EQ(reshaped->shape[1], (size_t)4);

    nc_free(&reshaped);
}
