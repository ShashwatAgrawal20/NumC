#ifndef NUMC_H
#define NUMC_H

#include "ndarray.h"
#include "ndarray_ops.h"

#define nc_dim_count(shape) (sizeof(shape) / sizeof((shape)[0]))
#define SND(shape) shape, nc_dim_count(shape)

#endif  // !NUMC_H
