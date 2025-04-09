#ifndef UTILS_H
#define UTILS_H

#define _check_fail() goto defer

#define _check_alloc(allocation)                                      \
    if (allocation == NULL) {                                         \
        fprintf(stderr, "malloc error: Memory allocation failed!\n"); \
        goto defer;                                                   \
    }

#define _check_null_return(item) \
    if (item == NULL) {          \
        return NULL;             \
    }

#define _check_zero(value) \
    if ((value) == 0) {    \
        return NULL;       \
    }

#define _log_and_return(msg)          \
    do {                              \
        fprintf(stderr, "%s\n", msg); \
        return NULL;                  \
    } while (0)

#define _GUARD(expr, msg)                 \
    do {                                  \
        if (expr) {                       \
            fprintf(stderr, "%s\n", msg); \
            return NULL;                  \
        }                                 \
    } while (0)

#endif  // !UTILS_H
