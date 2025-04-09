#include "../../include/slice.h"

#include "../../include/ndarray.h"
#include "../../include/utils.h"

ndarray_t *nc_slice(ndarray_t *array, slice_t *slices, int num_slices) {
    _GUARD(!array, "slice error: null array");
    _GUARD(num_slices != array->ndim, "slice error: dimension mismatch");

    ndarray_t *view = nc_create(array->shape, array->ndim, array->dtype);
    _GUARD(!view, "slice error: cannot create a view\n");
    free(view->data);
    view->owns_data = false;

    size_t offset = 0;
    for (int i = 0; i < array->ndim; ++i) {
        slice_t s = slices[i];

        if (s.end == 0 || s.end > array->shape[i]) s.end = array->shape[i];
        if (s.step == 0) s.step = 1;

        view->shape[i] = (s.end - s.start + s.step - 1) / s.step;
        view->strides[i] = array->strides[i] * s.step;

        offset += array->strides[i] * s.start;
    }
    view->total_size = 1;
    for (int i = 0; i < view->ndim; i++) view->total_size *= view->shape[i];

    view->data = (char *)array->data + offset;

    return view;
}
