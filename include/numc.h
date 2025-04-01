#ifndef NUMC_H
#define NUMC_H

#include "ndarray.h"
#include "ndarray_ops.h"

#define nc_dim_count(shape) (sizeof(shape) / sizeof((shape)[0]))

#endif  // !NUMC_H
