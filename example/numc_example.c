#include "numc/internal/broadcast.h"
#define NC_D_ORIW
#include <numc/numc.h>
#include <numc/pch.h>

int main(void) {
    printf("NumC: A NumPy-like numerical computing library in C.\n");
    /* FOR SINGLE DIMENSION */
    {
        // ndarray_t *a =
        //     nc_reshape(nc_arange(0, 3, 1, nc_double), SND_INLINE(3), true);
        // ndarray_t *b =
        //     nc_reshape(nc_arange(0, 6, 1, nc_double), SND_INLINE(2, 3),
        //     true);
        //
        // printf("nc_add -> ");
        // ndarray_t *c = nc_add(a, b);

        ndarray_t *a = nc_reshape(nc_arange(1, 7, 1, nc_double),
                                  SND_INLINE(2, 1, 3), true);

        // Create array b with shape (1,4,1)
        ndarray_t *b = nc_reshape(nc_arange(7, 11, 1, nc_double),
                                  SND_INLINE(1, 4, 1), true);

        printf("Array a shape: (2,1,3)\n");
        printf("Array b shape: (1,4,1)\n");

        // This should broadcast to result shape (2,4,3)
        ndarray_t *c = nc_add(a, b);
        printf("Result shape should be: (2,4,3)\n");

        nc_display(a, true);
        nc_display(b, true);
        nc_display(c, true);
        nc_free(&a);
        nc_free(&b);
        nc_free(&c);
    }

    return 0;

    {
        /* VALID */
        {
            {
                ndarray_t *a =
                    nc_reshape(nc_arange(0, 3, 1, nc_int), SND_INLINE(3), true);
                ndarray_t *b = nc_reshape(nc_arange(0, 3, 1, nc_int),
                                          SND_INLINE(1, 3), true);
                // Result shape: (1, 3) => broadcast to (1, 3)
                printf("%d\n", nc_can_broadcast(NULL, NULL));
                nc_free(&a);
                nc_free(&b);
            }

            {
                ndarray_t *a =
                    nc_reshape(nc_arange(0, 3, 1, nc_int), SND_INLINE(3), true);
                ndarray_t *b =
                    nc_reshape(nc_arange(0, 1, 1, nc_int), SND_INLINE(1), true);
                // b acts like scalar => broadcast to (3)
                printf("%d\n", nc_can_broadcast(a, b));
                nc_free(&a);
                nc_free(&b);
            }
            {
                ndarray_t *a = nc_reshape(nc_arange(0, 6, 1, nc_int),
                                          SND_INLINE(2, 3), true);
                ndarray_t *b = nc_reshape(nc_arange(0, 3, 1, nc_int),
                                          SND_INLINE(1, 3), true);
                // b broadcast to (2, 3)
                printf("%d\n", nc_can_broadcast(a, b));
                nc_free(&a);
                nc_free(&b);
            }
            {
                ndarray_t *a = nc_reshape(nc_arange(0, 2, 1, nc_float),
                                          SND_INLINE(2, 1, 1), true);
                ndarray_t *b = nc_reshape(nc_arange(0, 6, 1, nc_float),
                                          SND_INLINE(1, 3, 2), true);
                // broadcast to (2, 3, 2)
                printf("%d\n", nc_can_broadcast(a, b));
                nc_free(&a);
                nc_free(&b);
            }
            {
                ndarray_t *a = nc_reshape(nc_arange(0, 3, 1, nc_double),
                                          SND_INLINE(3), true);
                ndarray_t *b = nc_reshape(nc_arange(0, 6, 1, nc_double),
                                          SND_INLINE(2, 3), true);
                // ndim mismatch: can't broadcast (3) with (2,3)
                printf("%d\n", nc_can_broadcast(a, b));
                nc_free(&a);
                nc_free(&b);
            }
        }
        /* INVALID */
        {
            {
                ndarray_t *a = nc_reshape(nc_arange(0, 4, 1, nc_int),
                                          SND_INLINE(2, 2), true);
                ndarray_t *b =
                    nc_reshape(nc_arange(0, 3, 1, nc_int), SND_INLINE(3), true);
                // Shape mismatch at axis 1 (2 vs 3)
                printf("%d\n", nc_can_broadcast(a, b));
                nc_free(&a);
                nc_free(&b);
            }
            {
                ndarray_t *a = nc_reshape(nc_arange(0, 6, 1, nc_float),
                                          SND_INLINE(2, 3), true);
                ndarray_t *b = nc_reshape(nc_arange(0, 3, 1, nc_float),
                                          SND_INLINE(3, 1), true);
                // Incompatible shapes: (2, 3) vs (3, 1)
                printf("%d\n", nc_can_broadcast(a, b));
                nc_free(&a);
                nc_free(&b);
            }
        }
    }

    return 0;

    {
        // ndarray_t *test = nc_reshape(nc_arange(1, 100000001, 1,
        // nc_double),
        //                              SND_INLINE(100, 100, 100, 100),
        //                              true);
        ndarray_t *test =
            nc_reshape(nc_arange(1, 9, 1, nc_int), SND_INLINE(2, 2, 2), true);

        /* EXPLICIT SHIT */
        nc_sum_otps *opts = NC_SUM_DEFAULT_OPTS();
        opts->axis = 2;
        ndarray_t *sa_explicit = nc_sum(test, opts);

        /* VARIADIC */
        ndarray_t *sa_variadic =
            nc_sum(test, NC_SUM_DEFAULT_OPTS(.axis = 1, .dtype = nc_double));

        nc_display(test, true);
        nc_display(sa_explicit, true);
        nc_display(sa_variadic, true);

        nc_free(&test);
        nc_free(&sa_explicit);
        nc_free(&sa_variadic);
    }

    return 0;
    /*
    {
        ndarray_t *bs =
            nc_reshape(nc_arange(1, 7, 1, nc_int), SND_INLINE(2, 3),
    true); ndarray_t *bs1 = nc_reshape(nc_arange(1, 13, 1, nc_int),
    SND_INLINE(4, 3), true); ndarray_t *bs2 = nc_reshape(nc_arange(1, 7,
    1, nc_int), SND_INLINE(2, 3), true); ndarray_t *sall = nc_sum(bs,
    -1); ndarray_t *sall1 = nc_sum(bs1, 0); ndarray_t *sall2 =
    nc_sum(bs2, -1);

        // nc_display(bs, true);
        // nc_display(s0, true);
        // nc_display(s1, true);
        nc_display(sall, true);
        nc_display(sall1, true);
        nc_display(bs1, true);
        nc_display(sall2, true);
        // nc_free(&s0);
        // nc_free(&s1);
        nc_free(&sall);
        nc_free(&sall1);
        nc_free(&sall2);
        nc_free(&bs);
        nc_free(&bs1);
        nc_free(&bs2);
    }
    return 0;
    {
        size_t shape[] = {2, 2, 2, 2};
        ndarray_t *original =
            nc_reshape(nc_arange(0, 16, 1, nc_double), SND(shape),
    true);
        // nc_reshape(nc_arange(0, 16, 1, nc_double), SND_INLINE(2, 2,
    2, 2),
        // true);
        nc_display(original, true);
        nc_free(&original);
    }
    return 0;

    {
        size_t shape[] = {4, 5, 5};

        ndarray_t *original =
            nc_reshape(nc_arange(0, 100, 1, nc_int), SND(shape), true);

        slice_t slices[3] = {0};

        ndarray_t *sliced = nc_slice(original, SND(slices));
        nc_display(sliced, true);

        nc_free(&original);
        nc_free(&sliced);
    }
    */

    /* nc_slice example */
    {
        size_t shape[] = {20, 5};

        ndarray_t *original =
            nc_reshape(nc_arange(0, 100, 1, nc_int), SND(shape), true);

        slice_t slices[] = {
            {.start = 0, .step = 2},  // rows: take every other row
            {.start = 0, .step = 1}   // columns: take every column
        };

        ndarray_t *sliced = nc_slice(original, SND(slices));
        nc_display(sliced, true);

        nc_free(&original);
        nc_free(&sliced);
    }

    return 0;

    // ARRAY OF NDARRAYS
    {
#define NUM_OF_ARRAYS 2
#define NDIM 3
        size_t shapes[NUM_OF_ARRAYS][NDIM] = {{10, 1920, 1080}, {8, 16, 32}};
        dtype_t dtypes[NUM_OF_ARRAYS] = {nc_int, nc_float};

        ndarray_t *arrays[NUM_OF_ARRAYS] = {0};
        for (int i = 0; i < NUM_OF_ARRAYS; ++i) {
            arrays[i] = nc_create(shapes[i], NDIM, dtypes[i]);
            nc_display(arrays[i], false);
        }

        // nc_free(&arrays[1]);
        float value = 42.0f;
        size_t indices1[] = {7, 15, 31};
        size_t indices1_retrival[] = {7, 15, 31};
        nc_set(arrays[1], indices1, &value);

        void *retrieved = nc_get(arrays[1], indices1_retrival);
        if (retrieved) {
            printf("nc_get -> %f\n", *(float *)retrieved);
        } else {
            printf(
                "nc_get failed: index out of bounds or invalid "
                "access.\n");
        }
        for (int i = 0; i < NUM_OF_ARRAYS; ++i) {
            nc_free(&arrays[i]);
        }
    }

    // ARANGE INT
    {
        ndarray_t *fucking_trying_arange = nc_arange(0, 7000, 69, nc_int);
        nc_display(fucking_trying_arange, true);
        nc_free(&fucking_trying_arange);
    }

    // ARANGE FLOAT
    {
        ndarray_t *fucking_trying_arange_float =
            nc_arange(0, 7000, 69.69, nc_float);
        nc_display(fucking_trying_arange_float, false);
        nc_free(&fucking_trying_arange_float);
    }

    // RESHAPE THIS SHIT
    {
        size_t shape[] = {3, 4};
        size_t ndim = sizeof(shape) / sizeof(shape[0]);
        ndarray_t *fucking_trying_arange_int = nc_arange(0, 12, 1, nc_int);
        nc_display(fucking_trying_arange_int, true);
        ndarray_t *hehe_1 =
            nc_reshape(nc_arange(0, 12, 1, nc_int), shape, 2, true);
        /* IF YOU'RE DOING SHIT LIKE THIS JUST DON'T USE THE ANOTHER
         ARRAY IN
         * ANY CASE THAT WILL CAUSE UNCONDITIONAL SHIT AND STUFF.
            // nc_display(fucking_trying_arange_int, true);
            // nc_free(&fucking_trying_arange_int);
            // WILL RELEASE A DIFFERENT FUNCTION TO CHANGE THING INPLACE.
         */
        ndarray_t *hehe_2 =
            nc_reshape(fucking_trying_arange_int, shape, ndim, true);
        // printf("hehe_2 -> %p\n", (void *)hehe_2);
        nc_display(hehe_1, true);
        nc_display(hehe_2, true);
        nc_free(&hehe_1);
        nc_free(&hehe_2);
    }

    // OPERATION
    {
        /* FOR SINGLE DIMENSION */
        {
            ndarray_t *a = nc_arange(0, 50, 5, nc_int);
            ndarray_t *b = nc_arange(10, 20, 1, nc_int);

            printf("nc_add -> ");
            ndarray_t *c = nc_add(a, b);
            nc_display(c, true);
            nc_free(&a);
            nc_free(&b);
            nc_free(&c);
        }
        /* FOR MULTIDIMENSIONAL */
        {
            size_t shape[] = {3, 4};
            ndarray_t *hehe_1 =
                nc_reshape(nc_arange(0, 12, 1, nc_int), SND(shape), true);
            ndarray_t *hehe_2 = nc_reshape(nc_arange(0, 12, 1, nc_int), shape,
                                           nc_dim_count(shape), true);
            printf("nc_add multi dimensional-> ");
            ndarray_t *hehe_3 = nc_add(hehe_1, hehe_2);
            nc_display(hehe_3, true);

            nc_free(&hehe_1);
            nc_free(&hehe_2);
            nc_free(&hehe_3);
        }
        /* nc_mul SHIT */
        {
            size_t shape[] = {3, 4};
            ndarray_t *hehe_1 =
                nc_reshape(nc_arange(0, 12, 1, nc_int), shape, 2, true);
            ndarray_t *hehe_2 =
                nc_reshape(nc_arange(0, 12, 1, nc_int), shape, 2, true);
            ndarray_t *hehe_3 = nc_mul(hehe_1, hehe_2);
            printf("nc_mul -> ");
            nc_display(hehe_3, true);

            nc_free(&hehe_1);
            nc_free(&hehe_2);
            nc_free(&hehe_3);
        }
        /* nc_sub SHIT */
        {
            size_t shape[] = {3, 4};
            ndarray_t *hehe_1 = nc_reshape(nc_arange(0, 12, 1, nc_int), shape,
                                           nc_dim_count(shape), true);
            ndarray_t *hehe_2 = nc_reshape(nc_arange(0, 12, 1, nc_int), shape,
                                           nc_dim_count(shape), true);
            ndarray_t *hehe_3 = nc_sub(hehe_1, hehe_2);
            printf("nc_sub -> ");
            nc_display(hehe_3, true);

            nc_free(&hehe_1);
            nc_free(&hehe_2);
            nc_free(&hehe_3);
        }
        /* nc_div SHIT */
        {
            size_t shape[] = {2, 5};
            ndarray_t *hehe_1 = nc_reshape(nc_arange(100, 1100, 100, nc_int),
                                           shape, nc_dim_count(shape), true);
            ndarray_t *hehe_2 = nc_reshape(nc_arange(1, 21, 2, nc_int), shape,
                                           nc_dim_count(shape), true);
            // ndarray_t *hehe_2 = nc_reshape(nc_arange(10, 20, 1, nc_int),
            // shape,
            //                                nc_dim_count(shape),
            //                                true);
            ndarray_t *hehe_3 = nc_div(hehe_1, hehe_2);
            printf("nc_div -> ");
            nc_display(hehe_3, true);

            nc_free(&hehe_1);
            nc_free(&hehe_2);
            nc_free(&hehe_3);
        }
    }
    return 0;
}
