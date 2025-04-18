#include "numc.h"

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "internal/utils.h"

/*******************************************************************************
                               PRIVATE FUNCTIONS
*******************************************************************************/

static inline const char *_dtype_to_format(dtype_t type) {
    switch (type) {
        case nc_int:
            return "%d";
        case nc_float:
            return "%f";
        case nc_double:
            return "%lf";
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
            _ELOG("_dtype_size error: invalid dtype (%d)\n", dtype);
            return 0;
    }
}

static inline void _assign_value(void *ptr, double val, dtype_t dtype) {
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
            _ELOG("_assign_value error: invalid dtype (%d)\n", dtype);
    }
}

static inline void _nc_print_recursive(ndarray_t *array, int dim, size_t offset,
                                       int indent_level) {
#define CAST_AND_PRINT_ELEMS(TYPE)                                          \
    do {                                                                    \
        const char *format = _dtype_to_format((array->dtype));              \
        for (size_t i = 0; i < array->shape[dim]; ++i) {                    \
            size_t byte_offset = offset + i * array->strides[dim];          \
            const char *data_ptr = (const char *)array->data + byte_offset; \
            printf(format, *(TYPE *)data_ptr);                              \
            if (i < array->shape[dim] - 1) printf(", ");                    \
        }                                                                   \
    } while (0);

    if (dim == array->ndim - 1) {
        printf("%*s[", indent_level * 3, "");
        switch (array->dtype) {
            case nc_int:
                CAST_AND_PRINT_ELEMS(int);
                break;
            case nc_float:
                CAST_AND_PRINT_ELEMS(float);
                break;
            case nc_double:
                CAST_AND_PRINT_ELEMS(double);
                break;
        }
        printf("]");
    } else {
        printf("%*s[\n", indent_level * 3, "");
        for (size_t i = 0; i < array->shape[dim]; ++i) {
            size_t sub_offset = offset + i * array->strides[dim];
            _nc_print_recursive(array, dim + 1, sub_offset, indent_level + 1);
            if (i < array->shape[dim] - 1) {
                printf(",\n");  // Comma between sub-arrays
            }
        }
        printf("\n%*s]", (indent_level == 0) ? 2 : indent_level * 3, "");
    }
#undef CAST_AND_PRINT_ELEMS
}

static inline void _nc_print_all(ndarray_t *array) {
    if (!array || !array->data) {
        _ELOG("nc_display error: ");
        return;
    }

    _nc_print_recursive(array, 0, 0, 0);
}

static inline void _compute_strides(ndarray_t *array) {
    if (!array || array->ndim <= 0) return;

    array->strides[array->ndim - 1] = array->item_size;
    for (int i = array->ndim - 2; i >= 0; --i) {
        array->strides[i] = array->strides[i + 1] * array->shape[i + 1];
    }
}

static inline void _compute_total_size(ndarray_t *array) {
    if (!array) return;
    size_t total = 1;
    for (int i = 0; i < array->ndim; ++i) {
        total *= array->shape[i];
    }
    array->total_size = total;
}

/*******************************************************************************
                                PUBLIC FUNCTIONS
*******************************************************************************/
ndarray_t *nc_create(size_t *shape, int ndim, dtype_t dtype) {
    _GUARD(ndim <= 0,
           "nc_create error: can't initialize ndarray of dimention %d", ndim);
    size_t dtype_size = _dtype_size(dtype);
    _check_zero(dtype_size);

    ndarray_t *array = calloc(1, sizeof(ndarray_t));
    _check_alloc(array);

    array->shape = malloc(ndim * sizeof(size_t));
    array->strides = malloc(ndim * sizeof(size_t));
    array->owns_data = true;
    _check_alloc(array->shape);
    _check_alloc(array->strides);

    memcpy(array->shape, shape, ndim * sizeof(size_t));

    array->dtype = dtype;
    array->item_size = dtype_size;
    array->ndim = ndim;
    _compute_total_size(array);
    _compute_strides(array);
    // array->total_size = 1;
    // array->strides[ndim - 1] = dtype_size;

    // for (int i = ndim - 1; i >= 0; --i) {
    //     array->total_size *= shape[i];
    //     if (i > 0) {
    //         array->strides[i - 1] = array->strides[i] * shape[i];
    //     }
    // }

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

    if ((*array)->owns_data) {
        free((*array)->data);
    }

    // free((*array)->data);
    free((*array)->shape);
    free((*array)->strides);
    free((*array));
    *array = NULL;
}

void nc_set(ndarray_t *array, size_t *indices, void *value) {
    if (!array || !array->data) {
        _ELOG("nc_set error: array doesn't exists\n");
        return;
    }
    size_t offset = 0;
    for (int i = 0; i < array->ndim; ++i) {
        if (indices[i] >= array->shape[i]) {
            _ELOG(
                "nc_set error: index out of bounds: %zu (axis %d, shape %zu)\n",
                indices[i], i, array->shape[i]);
            return;
        }
        offset += indices[i] * array->strides[i];
    }

    memcpy((char *)array->data + offset, value, array->item_size);
}

void *nc_get(ndarray_t *array, size_t *indices) {
    if (!array || !array->data) return NULL;
    size_t offset = 0;
    for (int i = 0; i < array->ndim; ++i) {
        _GUARD(indices[i] >= array->shape[i],
               "nc_get error: index out of bounds: %zu (axis %d, shape %zu)\n",
               indices[i], i, array->shape[i]);
        offset += indices[i] * array->strides[i];
    }

    return (char *)array->data + offset;
}

ndarray_t *nc_arange(double start, double stop, double step, dtype_t dtype) {
    _GUARD(step == 0.0, "nd_arange error: step cannot be zero.\n");

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
        void *element = (char *)array->data + i * array->item_size;
        double value = start + i * step;

        _assign_value(element, value, dtype);
    }
    return array;
}

/**
 * @brief FUCK OFF CLANGD
 * Notes on `is_inline`:
 * Use `is_inline = true` ONLY when:
 *   * You're passing a temporary ndarray like `nc_arange(...)` directly into
 *     `nc_reshape(...)`, e.g.:
 *       nc_reshape(nc_arange(0, 12, 1, nc_int), shape, 2, true);
 *
 *   * Or you explicitly want to reshape in-place and understand that no new
 *     memory will be allocated.
 *
 * WARNING:
 * When `is_inline` is true, the reshaped array is the SAME pointer as the
 * original. This means:
 *   - Free only ONE of them — either the reshaped result or the original.
 *   - If you store both pointers, do not assume they're separate objects.
 *
 * 🚨 If you pass `is_inline=true` for an array stored in a **variable**
 *    (instead of a temporary), **it will be freed if an error occurs**. If
 *    there's no error, the reshape happens in-place.
 *
 * 🚨 **DO NOT** use `is_inline=true` on non-temporary arrays( unless you are
 *    100% sure you want the original array to be modified **AND** risk it being
 *    freed on failure.
 */
ndarray_t *nc_reshape(ndarray_t *array, size_t *shape, int ndim,
                      bool is_inline) {
    if (!array || ndim <= 0) {
        _ELOG("nc_reshape error: invalid input\n");
        _check_fail();
    }

    size_t new_total = 1;
    for (int i = 0; i < ndim; ++i) {
        new_total *= shape[i];
    }
    if (new_total != array->total_size) {
        _ELOG(
            "nc_reshape error: shape mismatch (original total: %zu, new: "
            "%zu)\n",
            array->total_size, new_total);
        _check_fail();
    }

    /*
     * TODO: this internal computation shit is getting ugly might make different
     * utility functions so that those will be used by both nc_create and here.
     */
    if (is_inline) {
        if (array->ndim != ndim) {
            size_t *new_shape = realloc(array->shape, ndim * sizeof(size_t));
            size_t *new_strides =
                realloc(array->strides, ndim * sizeof(size_t));
            if (!new_shape || !new_strides) {
                _ELOG("nc_reshape error: realloc failed\n");
                free(new_shape);
                free(new_strides);
                _check_fail();
            }
            array->shape = new_shape;
            array->strides = new_strides;
        }
        memcpy(array->shape, shape, ndim * sizeof(size_t));
        array->ndim = ndim;

        _compute_strides(array);

        return array;
    }

    ndarray_t *reshaped_array = nc_create(shape, ndim, array->dtype);
    _check_null_return(reshaped_array);
    memcpy(reshaped_array->data, array->data,
           array->total_size * array->item_size);

    return reshaped_array;

defer:
    if (is_inline) {
        if (array) nc_free(&array);
    }

    return NULL;
}

/**
 * Debug print for an ndarray_t struct.
 *
 * Parameters:
 *  * `array`: Pointer to the ndarray.
 *  * `print_data`: If true, prints data; else just metadata.
 *
 * Notes:
 *  * Data is printed as a 1D flat array.
 */
void nc_display(ndarray_t *array, bool print_data) {
    if (!array) {
        // printf("(null array)\");
        _ELOG("nc_display error: (null array)\n");
        return;
    }
    printf("ndarray_t:\n");
    printf("  ndim: %d\n", array->ndim);
    printf("  shape: [");
    for (int i = 0; i < array->ndim; ++i)
        printf("%zu%s", array->shape[i], (i < array->ndim - 1) ? ", " : "");
    printf("]\n");
    printf("  strides: [");
    for (int i = 0; i < array->ndim; ++i)
        printf("%zu%s", array->strides[i], (i < array->ndim - 1) ? ", " : "");
    printf("]\n");
    printf("  dtype: %d\n", array->dtype);
    printf("  total_size: %zu\n", array->total_size);
    printf("  item_size: %zu\n", array->item_size);
    /* AS OF NOW THIS SHIT WILL ONLY DISPALY DATA AS A 1D ARRAY BTW */
    if (!print_data) {
        printf("  data: [ WE DON'T NEED TO SEE THAT ]\n\n");
    } else {
        printf("  data:(");
        _nc_print_all(array);
        printf(")\n");
    }
}
