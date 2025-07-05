#ifndef BROADCAST_H
#define BROADCAST_H

#include <numc/pch.h>

#include "numc/core/ndarray.h"

bool nc_can_broadcast(const ndarray_t *a, const ndarray_t *b);

#endif  // !BROADCAST_H
