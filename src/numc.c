#include "../include/numc.h"

#include <stdio.h>

#define NUM_OF_ARRAYS 3
#define NDIM 3

int main() {
    printf("NumC: A NumPy-like numerical computing library in C.\n");

    size_t shapes[NUM_OF_ARRAYS][NDIM] = {
        {10, 1920, 1080}, {8, 16, 32}, {4, 4, 4}};
    dtype_t dtypes[NUM_OF_ARRAYS] = {INT32, FLOAT64, DOUBLE};

    ndarray_t *arrays[NUM_OF_ARRAYS] = {NULL, NULL, NULL};
    for (int i = 0; i < NUM_OF_ARRAYS; ++i) {
        arrays[i] = nc_create(shapes[i], NDIM, dtypes[i]);

        /*
         * FUCK IT
         */
        // if (!arrays[i]) {
        //     fprintf(stderr,
        //             "Error: Allocation failed for array %d, cleaning
        //             up...\n", i);
        //     for (int j = 0; j < i; j++) {
        //         nc_free(&arrays[j]);
        //     }
        //     return EXIT_FAILURE;
        // }

        nc_display(arrays[i]);
    }

    for (int i = 0; i < NUM_OF_ARRAYS; ++i) {
        nc_free(&arrays[i]);
    }

    return 0;
}
