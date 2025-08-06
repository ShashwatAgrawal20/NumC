#include <numc/numc.h>

#include "../../tec.h"

TEC(creation, test_arange_int) {
    ndarray_t *arr = nc_arange(0, 5, 1, nc_int);

    TEC_ASSERT(arr != NULL);
    TEC_ASSERT_EQ(arr->ndim, 1);
    TEC_ASSERT_EQ(arr->total_size, (size_t)5);
    TEC_ASSERT_EQ(arr->dtype, (dtype_t)nc_int);

    int *data = (int *)arr->data;
    for (int i = 0; i < 5; i++) {
        TEC_ASSERT_EQ(data[i], i);
    }

    nc_free(&arr);
    TEC_ASSERT_NULL(arr);
}

TEC(creation, test_arange_float) {
    ndarray_t *arr = nc_arange(0.0, 5.0, 0.5, nc_float);

    TEC_ASSERT(arr != NULL);
    TEC_ASSERT_EQ(arr->ndim, 1);
    TEC_ASSERT_EQ(arr->total_size, (size_t)10);
    TEC_ASSERT_EQ(arr->dtype, (dtype_t)nc_float);

    float *data = (float *)arr->data;
    TEC_ASSERT_EQ(data[0], 0.0f);
    TEC_ASSERT_EQ(data[1], 0.5f);
    TEC_ASSERT_EQ(data[2], 1.0f);

    nc_free(&arr);
}

TEC(creation, test_arange_double) {
    ndarray_t *arr = nc_arange(1.0, 4.0, 0.1, nc_double);

    TEC_ASSERT(arr != NULL);
    TEC_ASSERT_EQ(arr->dtype, (dtype_t)nc_double);
    TEC_ASSERT_EQ(arr->total_size, (size_t)30);

    double *data = (double *)arr->data;
    TEC_ASSERT_EQ(data[0], (double)1.0);
    TEC_ASSERT_EQ(data[10], (double)2.0);

    nc_free(&arr);
}

TEC(creation, test_arange_large_step) {
    ndarray_t *arr = nc_arange(0, 7000, 69, nc_int);

    TEC_ASSERT(arr != NULL);
    TEC_ASSERT_EQ(arr->total_size, (size_t)101);  // floor(7000/69) = 101

    int *data = (int *)arr->data;
    TEC_ASSERT_EQ(data[0], 0);
    TEC_ASSERT_EQ(data[1], 69);
    TEC_ASSERT_EQ(data[2], 138);

    nc_free(&arr);
}

TEC(creation, test_arange_negative_range) {
    ndarray_t *arr = nc_arange(-5, 5, 1, nc_int);

    TEC_ASSERT(arr != NULL);
    TEC_ASSERT_EQ(arr->total_size, (size_t)10);

    int *data = (int *)arr->data;
    TEC_ASSERT_EQ(data[0], -5);
    TEC_ASSERT_EQ(data[5], 0);
    TEC_ASSERT_EQ(data[9], 4);

    nc_free(&arr);
}

TEC(creation, test_create_multidimensional) {
    size_t shape[] = {2, 3, 4};
    ndarray_t *arr = nc_create(shape, 3, nc_int);

    TEC_ASSERT(arr != NULL);
    TEC_ASSERT_EQ(arr->ndim, 3);
    TEC_ASSERT_EQ(arr->total_size, (size_t)24);
    TEC_ASSERT_EQ(arr->shape[0], (size_t)2);
    TEC_ASSERT_EQ(arr->shape[1], (size_t)3);
    TEC_ASSERT_EQ(arr->shape[2], (size_t)4);

    nc_free(&arr);
}

TEC(creation, test_create_large_array) {
    size_t shape[] = {10, 1920, 1080};
    ndarray_t *arr = nc_create(shape, 3, nc_int);

    TEC_ASSERT(arr != NULL);
    TEC_ASSERT_EQ(arr->ndim, 3);
    TEC_ASSERT_EQ(arr->total_size, (size_t)20736000);

    nc_free(&arr);
}

TEC(creation, test_arange_edge_cases) {
    ndarray_t *empty = nc_arange(5, 5, 1, nc_int);
    TEC_ASSERT(empty != NULL);
    TEC_ASSERT_EQ(empty->total_size, (size_t)0);
    nc_free(&empty);

    ndarray_t *single = nc_arange(0, 1, 1, nc_int);
    TEC_ASSERT(single != NULL);
    TEC_ASSERT_EQ(single->total_size, (size_t)1);
    int *data = (int *)single->data;
    TEC_ASSERT_EQ(data[0], 0);
    nc_free(&single);
}
