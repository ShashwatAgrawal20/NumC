#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../include/numc.h"

/*******************************************************************************
                               PRIVATE FUNCTIONS
*******************************************************************************/

static inline const char *_dtype_to_format(dtype_t type) {
    switch (type) {
        case nc_int:
            return "%d ";
        case nc_float:
            return "%f ";
        case nc_double:
            return "%lf ";
        default:
            return NULL;
    }
}

static inline size_t _dtype_size(dtype_t dtype) {
    switch (dtype) {
        case nc_int:
            return sizeof(int);
        case nc_float:
            return sizeof(float);
        case nc_double:
            return sizeof(double);
        default:
            fprintf(stderr, "invalid dtype\n");
            return 0;
    }
}

static inline void assign_value(void *ptr, double val, dtype_t dtype) {
    switch (dtype) {
        case nc_int:
            *(int *)ptr = (int)val;
            break;
        case nc_float:
            *(float *)ptr = (float)val;
            break;
        case nc_double:
            *(double *)ptr = val;
            break;
        default:
            fprintf(stderr, "invalid dtype\n");
    }
}

static inline void _nc_print_all(ndarray_t *array) {
    if (!array) return;

    const char *format = _dtype_to_format((array->dtype));
    switch (array->dtype) {
        case nc_int: {
            int *data = (int *)array->data;
            for (size_t i = 0; i < array->total_size; ++i) {
                printf(format, data[i]);
            }
            break;
        }
        case nc_float: {
            float *data = (float *)array->data;
            for (size_t i = 0; i < array->total_size; ++i) {
                printf(format, data[i]);
            }
            break;
        }
        case nc_double: {
            double *data = (double *)array->data;
            for (size_t i = 0; i < array->total_size; ++i) {
                printf(format, data[i]);
            }
            break;
        }
    }
}

/*******************************************************************************
                                PUBLIC FUNCTIONS
*******************************************************************************/
ndarray_t *nc_create(size_t *shape, int ndim, dtype_t dtype) {
    if (ndim <= 0) {
        fprintf(stderr,
                "nc_create error: can't initialize ndarray of dimention %d",
                ndim);
        return NULL;
    }
    size_t dtype_size = _dtype_size(dtype);
    _check_zero(dtype_size);

    ndarray_t *array = calloc(1, sizeof(ndarray_t));
    _check_alloc(array);

    array->shape = malloc(ndim * sizeof(size_t));
    array->strides = malloc(ndim * sizeof(size_t));
    _check_alloc(array->shape);
    _check_alloc(array->strides);

    memcpy(array->shape, shape, ndim * sizeof(size_t));

    array->dtype = dtype;
    array->item_size = dtype_size;
    array->ndim = ndim;
    array->total_size = 1;
    array->strides[ndim - 1] = dtype_size;

    for (int i = ndim - 1; i >= 0; --i) {
        array->total_size *= shape[i];
        if (i > 0) {
            array->strides[i - 1] = array->strides[i] * shape[i];
        }
    }

    array->data = calloc(array->total_size, dtype_size);
    _check_alloc(array->data);

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

void nc_set(ndarray_t *array, size_t *indices, void *value) {
    if (!array || !array->data) {
        fprintf(stderr, "nc_set error: array doesn't exists\n");
        return;
    }
    size_t offset = 0;
    for (int i = 0; i < array->ndim; i++) {
        if (indices[i] >= array->shape[i]) {
            fprintf(
                stderr,
                "nc_set error: index out of bounds: %zu (axis %d, shape %zu)\n",
                indices[i], i, array->shape[i]);
            return;
        }
        // printf("\t%zu, ", indices[i]);
        offset += indices[i] * array->strides[i];
    }
    // These logs are meant to go but keeping these as of now for some debug
    // purposes.
    // printf("writing on -> %zu\n", offset);
    // printf(
    //     "nc_set error: writing at offset %zu (total size: %zu | total "
    //     "elements: "
    //     "%zu)\n",
    //     offset, array->total_size * array->item_size, array->total_size);

    memcpy((char *)array->data + offset, value, array->item_size);
}

void *nc_get(ndarray_t *array, size_t *indices) {
    if (!array || !array->data) return NULL;
    size_t offset = 0;
    for (int i = 0; i < array->ndim; i++) {
        if (indices[i] >= array->shape[i]) {
            fprintf(
                stderr,
                "nc_get error: index out of bounds: %zu (axis %d, shape %zu)\n",
                indices[i], i, array->shape[i]);
            return NULL;
        }
        offset += indices[i] * array->strides[i];
    }

    return (char *)array->data + offset;
}

ndarray_t *nc_arange(double start, double stop, double step, dtype_t dtype) {
    if (step == 0.0) {
        fprintf(stderr, "nd_arange error: step cannot be zero.\n");
        return NULL;
    }

    /*
        TODO: I DON'T LIKE THIS CEIL SHIT CAUSE I'VE TO MANUALLY DO THAT LM SHIT
        WHILE BUILDING AND PROBABLY WILL IMPLEMENT THIS WITH LOOPS AND STUFF
        LATER.
    */
    size_t length = (size_t)ceil((stop - start) / step);
    if ((step > 0 && start >= stop) || (step < 0 && start <= stop)) {
        length = 0;
    }

    size_t shape[1] = {length};
    ndarray_t *array = nc_create(shape, 1, dtype);
    _check_null_return(array);

    for (size_t i = 0; i < length; ++i) {
        void *element = array->data + i * array->item_size;
        double value = start + i * step;

        assign_value(element, value, dtype);
    }
    return array;
}

ndarray_t *nc_reshape(ndarray_t *array, size_t *shape, int ndim) {
    if (!array || ndim <= 0) {
        fprintf(stderr, "nc_reshape error: invalid input\n");
        return NULL;
    }

    size_t new_total = 1;
    for (int i = 0; i < ndim; ++i) {
        new_total *= shape[i];
    }
    if (new_total != array->total_size) {
        fprintf(stderr,
                "nc_reshape error: shape mismatch (original total: %zu, new: "
                "%zu)\n",
                array->total_size, new_total);
        return NULL;
    }
    ndarray_t *reshaped_array = nc_create(shape, ndim, array->dtype);
    _check_null_return(reshaped_array);
    memcpy(reshaped_array->data, array->data,
           array->total_size * array->item_size);

    return reshaped_array;
}

/*
 * We don't need this btw it's just for development purposes but idk might
 * leave this here.
 * */
void nc_display(ndarray_t *array, bool print_data) {
    if (!array) {
        printf("(null array)\n");
        return;
    }
    printf("ndarray_t:\n");
    printf("  ndim: %d\n", array->ndim);
    printf("  shape: [");
    for (int i = 0; i < array->ndim; i++)
        printf("%zu%s", array->shape[i], (i < array->ndim - 1) ? ", " : "");
    printf("]\n");
    printf("  strides: [");
    for (int i = 0; i < array->ndim; i++)
        printf("%zu%s", array->strides[i], (i < array->ndim - 1) ? ", " : "");
    printf("]\n");
    printf("  dtype: %d\n", array->dtype);
    printf("  total_size: %zu\n", array->total_size);
    printf("  item_size: %zu\n", array->item_size);
    /* AS OF NOW THIS SHIT WILL ONLY DISPALY DATA AS A 1D ARRAY BTW */
    if (!print_data) {
        printf("  data: [ WE DON'T NEED TO SEE THAT ]\n\n");
    } else {
        printf("  data: [ ");
        _nc_print_all(array);
        printf("] \n\n");
    }
}
