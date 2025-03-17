#include <stdlib.h>
#include <string.h>

#include "../include/numc.h"

ndarray_t *nc_create(size_t *shape, int ndim, dtype_t dtype) {
    if (ndim <= 0) {
        fprintf(stderr, "can't initialize ndarray of dimention %d", ndim);
        return NULL;
    }
    // (void)(shape);
    (void)(dtype);

    ndarray_t *array = calloc(1, sizeof(ndarray_t));
    _check_alloc(array);
    // memset(array, 0, sizeof(ndarray_t));
    array->shape = malloc(ndim * sizeof(size_t));
    // array->strides = NULL;
    array->strides = malloc(ndim * sizeof(size_t));
    _check_alloc(array->shape);
    _check_alloc(array->strides);
    memcpy(array->shape, shape, ndim * sizeof(size_t));
    // goto defer;
    // goto defer;
    // goto defer;
    // goto defer;

    return array;

defer:
    if (array) {
        free(array->data);
        free(array->shape);
        free(array->strides);
        free(array);
    }
    return NULL;
}

void nc_free(ndarray_t **array) {
    if (!array || !(*array)) return;

    free((*array)->data);
    free((*array)->shape);
    free((*array)->strides);
    free((*array));
    *array = NULL;
}
