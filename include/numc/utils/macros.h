#ifndef MACROS_H
#define MACROS_H

#define nc_dim_count(shape) (sizeof(shape) / sizeof((shape)[0]))
#define SND(shape) shape, nc_dim_count(shape)
#define SND_INLINE(...) \
    ((size_t[]){__VA_ARGS__}), sizeof((size_t[]){__VA_ARGS__}) / sizeof(size_t)

#endif  // !MACROS_H
