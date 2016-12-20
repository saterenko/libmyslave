#ifndef MYSLAVE_UTILS_H
#define MYSLAVE_UTILS_H

#include "cor_pool.h"

typedef struct
{
    int size;
    char *data;
} myslave_str_t;

#define myslave_str_null {0, NULL}
#define myslave_str_init(_s) {sizeof(_s) - 1, _s}

int myslave_copy_str(myslave_str_t *dst, const char *src, int size, cor_pool_t *pool);

#endif
