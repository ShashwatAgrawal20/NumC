#include "../include/numc.h"

#include <stdio.h>

#define NUM_OF_ARRAYS 2
#define NDIM 3

int main() {
    printf("NumC: A NumPy-like numerical computing library in C.\n");

    size_t shapes[NUM_OF_ARRAYS][NDIM] = {{10, 1920, 1080}, {8, 16, 32}};
    dtype_t dtypes[NUM_OF_ARRAYS] = {nc_int, nc_float};

    ndarray_t *arrays[NUM_OF_ARRAYS] = {0};
    for (int i = 0; i < NUM_OF_ARRAYS; ++i) {
        arrays[i] = nc_create(shapes[i], NDIM, dtypes[i]);
        nc_display(arrays[i]);
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

    // ARANGE INT
    {
        ndarray_t *fucking_trying_arange = nc_arange(0, 7000, 69, nc_int);
        nc_display(fucking_trying_arange);
        for (size_t i = 0; i < fucking_trying_arange->total_size; ++i) {
            printf("%d ", ((int *)fucking_trying_arange->data)[i]);
        }
        printf("\n");
        nc_free(&fucking_trying_arange);
    }

    // ARANGE FLOAT
    {
        ndarray_t *fucking_trying_arange_float =
            nc_arange(0, 7000, 69.69, nc_float);
        nc_display(fucking_trying_arange_float);
        for (size_t i = 0; i < fucking_trying_arange_float->total_size; ++i) {
            printf("%f ", ((float *)fucking_trying_arange_float->data)[i]);
        }
        printf("\n");

        nc_free(&fucking_trying_arange_float);
    }

    for (int i = 0; i < NUM_OF_ARRAYS; ++i) {
        nc_free(&arrays[i]);
    }

    return 0;
}
