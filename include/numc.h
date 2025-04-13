#ifndef NUMC_H
#define NUMC_H

#include "core/ndarray.h"
#include "core/slice.h"
#include "ops/basic_ops.h"
#include "ops/reduction_ops.h"

#define nc_dim_count(shape) (sizeof(shape) / sizeof((shape)[0]))
#define SND(shape) shape, nc_dim_count(shape)

#endif  // !NUMC_H
