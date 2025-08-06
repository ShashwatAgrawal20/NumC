#include <numc/numc.h>

#include "../../tec.h"

TEC(indexing, test_slice_basic) {
    size_t shape[] = {20, 5};
    ndarray_t *original =
        nc_reshape(nc_arange(0, 100, 1, nc_int), shape, 2, true);

    slice_t slices[] = {
        {.start = 0, .step = 2},  // every other row
        {.start = 0, .step = 1}   // every column
    };

    ndarray_t *sliced = nc_slice(original, SND(slices));

    TEC_ASSERT(sliced != NULL);
    TEC_ASSERT_EQ(sliced->shape[1], (size_t)5);   // columns unchanged
    TEC_ASSERT_EQ(sliced->shape[0], (size_t)10);  // half the rows (every other)

    nc_free(&original);
    nc_free(&sliced);
}

TEC(indexing, test_slice_3d) {
    size_t shape[] = {4, 5, 5};
    ndarray_t *original =
        nc_reshape(nc_arange(0, 100, 1, nc_int), shape, 3, true);

    slice_t slices[3] = {0};  // Default slices
    ndarray_t *sliced = nc_slice(original, SND(slices));

    TEC_ASSERT(sliced != NULL);

    nc_free(&original);
    nc_free(&sliced);
}

TEC(indexing, test_get_set_operations) {
    size_t shape[] = {8, 16, 32};
    ndarray_t *arr = nc_create(shape, 3, nc_float);

    float value = 42.0f;
    size_t indices[] = {7, 15, 31};

    // Test set
    nc_set(arr, indices, &value);

    // Test get
    void *retrieved = nc_get(arr, indices);
    TEC_ASSERT(retrieved != NULL);
    TEC_ASSERT_EQ(*(float *)retrieved, 42.0f);

    nc_free(&arr);
}

TEC(indexing, test_get_out_of_bounds) {
    size_t shape[] = {2, 2};
    ndarray_t *arr = nc_create(shape, 2, nc_int);

    size_t invalid_indices[] = {3, 3};  // Out of bounds
    void *retrieved = nc_get(arr, invalid_indices);

    TEC_ASSERT(retrieved == NULL);  // Should return NULL for out of bounds

    nc_free(&arr);
}
