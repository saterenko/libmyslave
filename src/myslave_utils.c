#include "myslave_utils.h"

int
myslave_copy_str(myslave_str_t *dst, const char *src, int size, cor_pool_t *pool)
{
    dst->data = (char *) cor_pool_alloc(pool, size + 1);
    if (!dst->data) {
        return -1;
    }
    memcpy(dst->data, src, size);
    dst->data[size] = '\0';
    dst->size = size;
    return 0;
}

