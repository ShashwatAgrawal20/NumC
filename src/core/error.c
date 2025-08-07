#include "numc/core/error.h"

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#define NC_THREAD_LOCAL thread_local
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#define NC_THREAD_LOCAL _Thread_local
#else
#define NC_THREAD_LOCAL static
#endif

NC_THREAD_LOCAL nc_error_t g_last_error = NC_SUCCESS;

void _nc_set_error(nc_error_t err) { g_last_error = err; }

nc_error_t nc_get_last_error(void) { return g_last_error; }

// THIS IS THE CORRECTED, COMPLETE FUNCTION
const char* nc_strerror(nc_error_t err) {
    switch (err) {
        case NC_SUCCESS:
            return "Success";
        case NC_ERR_NULL_INPUT:
            return "Null input provided to function";
        case NC_ERR_MEM_ALLOC:
            return "Memory allocation failed";
        case NC_ERR_BROADCAST:
            return "Arrays could not be broadcast together";
        case NC_ERR_DIM_MISMATCH:
            return "Dimension mismatch";
        case NC_ERR_DTYPE_MISMATCH:
            return "Data type mismatch";
        case NC_ERR_DIVISION_BY_ZERO:
            return "Division by zero";
        case NC_ERR_OUT_OF_BOUNDS:
            return "Index out of bounds";
        case NC_ERR_INVALID_ARG:
            return "Invalid argument provided";
        case NC_ERR_UNSUPPORTED_DTYPE:
            return "Unsupported data type";
        case NC_ERR_RESHAPE_MISMATCH:
            return "Total size of new shape must match original";
        case NC_ERR_UNKNOWN:
        default:
            return "Unknown error";
    }
}
