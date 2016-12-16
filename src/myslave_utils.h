#ifndef MYSLAVE_UTILS_H
#define MYSLAVE_UTILS_H

#include "cor_pool.h"

typedef struct
{
    int size;
    char *data;
} myslave_str_t;

inline int
myslave_copy_str(myslave_str_t *dst, const char *src, int size, cor_pool_t *pool)
{
    dst->size = size;
    dst->data = (char *) cor_pool_alloc(pool, size + 1);
    if (!dst->data) {
        return -1;
    }
    memcpy(dst->data, src, size);
    dst->data[size] = '\0';
    return 0;
}


#endif
