#ifndef NUMC_ERROR_H
#define NUMC_ERROR_H

typedef enum {
    NC_SUCCESS = 0,
    NC_ERR_NULL_INPUT,
    NC_ERR_MEM_ALLOC,
    NC_ERR_BROADCAST,
    NC_ERR_DIM_MISMATCH,
    NC_ERR_DTYPE_MISMATCH,
    NC_ERR_DIVISION_BY_ZERO,
    NC_ERR_OUT_OF_BOUNDS,
    NC_ERR_INVALID_ARG,
    NC_ERR_UNSUPPORTED_DTYPE,
    NC_ERR_RESHAPE_MISMATCH,
    NC_ERR_UNKNOWN
} nc_error_t;

nc_error_t nc_get_last_error(void);
const char* nc_strerror(nc_error_t err);

#endif  // !NUMC_ERROR_H
