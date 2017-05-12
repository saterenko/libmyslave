#ifndef COLLATION_H
#define COLLATION_H

#include "cor_mysql.h"
#include "cor_pool.h"
#include "cor_trace.h"

typedef struct
{
    cor_str_t name;
    int size;
} myslave_collation_t;

typedef struct
{

} myslave_collations_t;

myslave_collations_t *myslave_collations_load(cor_mysql_t *m, cor_pool_t *pool, cor_trace_t *trace);
void myslave_collations_get(const char *name, size_t size);

#endif
