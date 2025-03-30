#include "../include/numc.h"

#include <stdio.h>

#define NUM_OF_ARRAYS 2
#define NDIM 3

int main() {
    printf("NumC: A NumPy-like numerical computing library in C.\n");

    // ARRAY OF NDARRAYS
    {
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
        ndarray_t *fucking_trying_arange_int = nc_arange(0, 12, 1, nc_int);
        nc_display(fucking_trying_arange_int, true);
        ndarray_t *hehe_1 =
            nc_reshape(nc_arange(0, 12, 1, nc_int), shape, 2, true);
        ndarray_t *hehe_2 =
            nc_reshape(fucking_trying_arange_int, shape, 2, true);
        nc_display(hehe_1, true);
        nc_display(hehe_2, true);
        nc_display(fucking_trying_arange_int, true);
        nc_free(&hehe_1);
        nc_free(&hehe_2);
        nc_free(&hehe_2);
        // nc_free(&fucking_trying_arange_int);
    }

    // ADD OPERATION
    {
        /* FOR SINGLE DIMENSION */
        {
            ndarray_t *a = nc_arange(0, 50, 5, nc_int);
            ndarray_t *b = nc_arange(10, 20, 1, nc_int);

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
                nc_reshape(nc_arange(0, 12, 1, nc_int), shape, 2, true);
            ndarray_t *hehe_2 =
                nc_reshape(nc_arange(0, 12, 1, nc_int), shape, 2, true);
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
            nc_display(hehe_3, true);

            nc_free(&hehe_1);
            nc_free(&hehe_2);
            nc_free(&hehe_3);
        }
    }

    return 0;
}
