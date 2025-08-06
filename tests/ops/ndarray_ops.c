#include <numc/numc.h>

#include "../../tec.h"

TEC(ops, test_add_1d_arrays) {
    ndarray_t *a = nc_arange(0, 50, 5, nc_int);
    ndarray_t *b = nc_arange(10, 20, 1, nc_int);

    ndarray_t *c = nc_add(a, b);

    TEC_ASSERT(c != NULL);
    TEC_ASSERT_EQ(c->ndim, 1);
    TEC_ASSERT_EQ(c->total_size, (size_t)10);

    int *data_a = (int *)a->data;
    int *data_b = (int *)b->data;
    int *data_c = (int *)c->data;

    // Check first few elements
    TEC_ASSERT_EQ(data_c[0], data_a[0] + data_b[0]);
    TEC_ASSERT_EQ(data_c[1], data_a[1] + data_b[1]);

    nc_free(&a);
    nc_free(&b);
    nc_free(&c);
}

TEC(ops, test_add_2d_arrays) {
    size_t shape[] = {3, 4};
    ndarray_t *a = nc_reshape(nc_arange(0, 12, 1, nc_int), shape, 2, true);
    ndarray_t *b = nc_reshape(nc_arange(0, 12, 1, nc_int), shape, 2, true);

    ndarray_t *c = nc_add(a, b);

    TEC_ASSERT(c != NULL);
    TEC_ASSERT_EQ(c->ndim, 2);
    TEC_ASSERT_EQ(c->shape[0], (size_t)3);
    TEC_ASSERT_EQ(c->shape[1], (size_t)4);

    int *data = (int *)c->data;
    TEC_ASSERT_EQ(data[0], 0);    // 0 + 0
    TEC_ASSERT_EQ(data[1], 2);    // 1 + 1
    TEC_ASSERT_EQ(data[11], 22);  // 11 + 11

    nc_free(&a);
    nc_free(&b);
    nc_free(&c);
}

TEC(ops, test_mul_arrays) {
    size_t shape[] = {3, 4};
    ndarray_t *a = nc_reshape(nc_arange(0, 12, 1, nc_int), shape, 2, true);
    ndarray_t *b = nc_reshape(nc_arange(0, 12, 1, nc_int), shape, 2, true);

    ndarray_t *c = nc_mul(a, b);

    TEC_ASSERT(c != NULL);
    TEC_ASSERT_EQ(c->ndim, 2);

    int *data = (int *)c->data;
    TEC_ASSERT_EQ(data[0], 0);  // 0 * 0
    TEC_ASSERT_EQ(data[1], 1);  // 1 * 1
    TEC_ASSERT_EQ(data[2], 4);  // 2 * 2
    TEC_ASSERT_EQ(data[3], 9);  // 3 * 3

    nc_free(&a);
    nc_free(&b);
    nc_free(&c);
}

TEC(ops, test_sub_arrays) {
    size_t shape[] = {3, 4};
    ndarray_t *a = nc_reshape(nc_arange(0, 12, 1, nc_int), shape, 2, true);
    ndarray_t *b = nc_reshape(nc_arange(0, 12, 1, nc_int), shape, 2, true);

    ndarray_t *c = nc_sub(a, b);

    TEC_ASSERT(c != NULL);
    TEC_ASSERT_EQ(c->ndim, 2);

    int *data = (int *)c->data;
    // All elements should be 0 since a - a = 0
    for (size_t i = 0; i < c->total_size; i++) {
        TEC_ASSERT_EQ(data[i], 0);
    }

    nc_free(&a);
    nc_free(&b);
    nc_free(&c);
}

TEC(ops, test_div_arrays) {
    size_t shape[] = {2, 5};
    ndarray_t *a =
        nc_reshape(nc_arange(100, 1100, 100, nc_int), shape, 2, true);
    ndarray_t *b = nc_reshape(nc_arange(1, 21, 2, nc_int), shape, 2, true);

    ndarray_t *c = nc_div(a, b);

    TEC_ASSERT(c != NULL);
    TEC_ASSERT_EQ(c->ndim, 2);
    TEC_ASSERT_EQ(c->shape[0], (size_t)2);
    TEC_ASSERT_EQ(c->shape[1], (size_t)5);

    nc_free(&a);
    nc_free(&b);
    nc_free(&c);
}
