#include "../include/numc.h"

#include <stdio.h>

#define NUM_OF_ARRAYS 3
int main() {
    printf("NumC: A NumPy-like numerical computing library in C.\n");
    size_t shape[] = {2, 3};
    // ndarray_t *array = nc_create(shape, 2, INT32);
    // ndarray_t *array1 = nc_create(shape, 2, INT32);
    // ndarray_t *array2 = nc_create(shape, 2, INT32);
    // // _check_null_return(array);
    //
    // nc_free(&array);
    // nc_free(&array1);
    // nc_free(&array2);
    // nc_free(&array);
    // nc_free(&array1);
    // nc_free(&array2);

    ndarray_t *arrays[NUM_OF_ARRAYS] = {NULL, NULL, NULL};
    for (int i = 0; i < NUM_OF_ARRAYS; i++) {
        arrays[i] = nc_create(shape, 2, INT32);
        if (!arrays[i]) {
            fprintf(stderr,
                    "Error: Allocation failed for array %d, cleaning up...\n",
                    i);
            for (int j = 0; j < i; j++) {
                nc_free(&arrays[j]);
            }
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < NUM_OF_ARRAYS; i++) {
        nc_free(&arrays[i]);
    }

    return 0;
}
