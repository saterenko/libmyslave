#ifndef COR_STR_H
#define COR_STR_H

#include "cor_pool.h"

typedef struct
{
    size_t size;
    char *data;
} cor_str_t;

#define cor_str(_s) { sizeof(_s) - 1, _s }
#define cor_str_null  { 0, NULL }
#define cor_str_tolower(_c) (_c >= 'A' && _c <= 'Z') ? (_c | 0x20) : _c
#define cor_str_toupper(_c) (_c >= 'a' && _c <= 'z') ? (_c & ~0x20) : _c

cor_str_t *cor_str_pnew(cor_pool_t *pool, size_t size);
cor_str_t *cor_str_pnew_cptr(cor_pool_t *pool, const char *src, size_t size);

static inline int
cor_str_pcopy_cptr(cor_pool_t *pool, cor_str_t *dst, const char *src, int size)
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

static inline int
cor_str_itoa(int n, char *buf)
{
    char *k = buf;
    int i = n;
    do {
        ++k;
    } while (i /= 10);
    int size = k - buf;
    do {
        (*--k) = n % 10 + '0';
    } while (n /= 10);

    return size;
}

void cor_str_utf8_to_lower(char *src, int size);

#endif
