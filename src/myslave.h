#ifndef MYSLAVE_H
#define MYSLAVE_H

#include "myslave_table.h"

typedef struct myslave_s myslave_t;
typedef void (myslave_handler_t) (const char *table, enum myslave_event_e event, myslave_field_t **fields);


myslave_t *myslave_new(const char *host, int port, const char *user, const char *pwd, myslave_handler_t *handler, void *arg);
const char *myslave_get_error(myslave_t *my);
void myslave_delete(myslave_t *my);
int myslave_add_table(myslave_t *my, const char *db, const char *table);
int myslave_run(myslave_t *my);

#endif
