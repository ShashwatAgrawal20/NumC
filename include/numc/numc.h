#ifndef NUMC_H
#define NUMC_H

#include <numc/core/ndarray.h>
#include <numc/core/slice.h>
#include <numc/ops/basic_ops.h>
#include <numc/ops/reduction_ops.h>

#define nc_dim_count(shape) (sizeof(shape) / sizeof((shape)[0]))
#define SND(shape) shape, nc_dim_count(shape)
#define SND_INLINE(...) \
    ((size_t[]){__VA_ARGS__}), sizeof((size_t[]){__VA_ARGS__}) / sizeof(size_t)

#endif  // !NUMC_H
