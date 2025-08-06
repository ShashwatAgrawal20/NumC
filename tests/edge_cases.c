#include <numc/internal/broadcast.h>
#include <numc/numc.h>

#include "../tec.h"

TEC(error_handling, test_null_pointer_broadcast) {
    bool result = nc_can_broadcast(NULL, NULL);
    TEC_ASSERT(result == false);
}

TEC(error_handling, test_null_input_to_ops) {
    ndarray_t *a = nc_arange(0, 10, 1, nc_int);

    TEC_ASSERT_NULL(nc_add(a, NULL));
    TEC_ASSERT_NULL(nc_add(NULL, a));
    TEC_ASSERT_NULL(nc_add(NULL, NULL));

    nc_free(&a);
}

TEC(error_handling, test_division_by_zero) {
    ndarray_t *a_int = nc_arange(0, 5, 1, nc_int);
    ndarray_t *b_int = nc_create(SND_INLINE(5), nc_int);
    ((int *)b_int->data)[2] = 0;

    ndarray_t *result = nc_div(a_int, b_int);

    TEC_ASSERT_NULL(result);

    nc_free(&a_int);
    nc_free(&b_int);
}

TEC(edge_cases, test_large_array_operations) {
    ndarray_t *a = nc_arange(0, 1000, 1, nc_int);
    ndarray_t *b = nc_arange(1000, 2000, 1, nc_int);

    ndarray_t *c = nc_add(a, b);

    TEC_ASSERT_NOT_NULL(c);
    TEC_ASSERT_EQ(c->total_size, (size_t)1000);

    int *data = (int *)c->data;
    TEC_ASSERT_EQ(data[0], 1000);    // 0 + 1000
    TEC_ASSERT_EQ(data[999], 2998);  // 999 + 1999

    nc_free(&a);
    nc_free(&b);
    nc_free(&c);
}
