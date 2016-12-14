#ifndef MYSLAVE_H
#define MYSLAVE_H

#include "cor_array.h"
#include "cor_trace.h"

#include "myslave_table.h"

#define MYSLAVE_ERROR_SIZE 1024

typedef void (myslave_handler_t) (const char *table, enum myslave_event_e event, myslave_field_t **fields);

typedef struct
{
    char *host;
    int port;
    char *user;
    char *pwd;
    myslave_handler_t *handler;
    void *arg;
    /**/
    cor_array_t tables;
    cor_pool_t *pool;
    cor_trace_t *t;
    char error[MYSLAVE_ERROR_SIZE + 1];
} myslave_t;


myslave_t *myslave_new(const char *host, int port, const char *user, const char *pwd, myslave_handler_t *handler, void *arg);
const char *myslave_get_error(myslave_t *my);
void myslave_delete(myslave_t *my);
int myslave_add_table(myslave_t *my, const char *db, const char *table);
int myslave_run(myslave_t *my);

#endif
