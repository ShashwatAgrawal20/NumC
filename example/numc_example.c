#include <stdio.h>

#include "numc.h"

int main(void) {
    printf("NumC: A NumPy-like numerical computing library in C.\n");

    {
        ndarray_t *bs = nc_arange(1, 2, 1, nc_int);
        nc_display(bs, true);
        nc_free(&bs);
    }
    {
        size_t shape[] = {2, 2, 2, 2};
        ndarray_t *original =
            nc_reshape(nc_arange(0, 16, 1, nc_double), SND(shape), true);
        nc_display(original, true);
        nc_free(&original);
    }
    // return 0;

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
            printf("nc_get failed: index out of bounds or invalid access.\n");
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
        /* IF YOU'RE DOING SHIT LIKE THIS JUST DON'T USE THE ANOTHER ARRAY IN
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
            //                                nc_dim_count(shape), true);
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
