#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mysql.h>
#include <my_global.h>
#include <m_ctype.h>
#include <sql_common.h>

#include "cor_mysql.h"
#include "myslave.h"

#define MYSLAVE_POOL_SIZE 8 * 1024

int myslave_mysql_check(myslave_t *my, cor_mysql_t *m);
int myslave_mysql_version_to_int(const char *p);
int myslave_load_table_info(myslave_t *my, cor_mysql_t *m);

myslave_t *
myslave_new(const char *host, int port, const char *user, const char *pwd, myslave_handler_t *handler, void *arg)
{
    cor_pool_t *pool = cor_pool_new(MYSLAVE_POOL_SIZE);
    if (!pool) {
        return NULL;
    }
    myslave_t *my = (myslave_t *) cor_pool_calloc(pool, sizeof(myslave_t));
    if (!my) {
        cor_pool_delete(pool);
        return NULL;
    }
    my->pool = pool;
    my->port = port;
    my->handler = handler;
    my->arg = arg;
    /*  copy host  */
    if (!host || strlen(host) == 0) {
        cor_pool_delete(pool);
        return NULL;
    }
    if (myslave_copy_str(&my->host, host, strlen(host), pool) != 0) {
        cor_pool_delete(pool);
        return NULL;
    }
    /*  copy user  */
    if (!user || strlen(user) == 0) {
        cor_pool_delete(pool);
        return NULL;
    }
    if (myslave_copy_str(&my->user, user, strlen(user), pool) != 0) {
        cor_pool_delete(pool);
        return NULL;
    }
    /*  copy password  */
    if (pwd && strlen(pwd) > 0) {
        if (myslave_copy_str(&my->pwd, pwd, strlen(pwd), pool) != 0) {
            cor_pool_delete(pool);
            return NULL;
        }
    }
    /*  init tables  */
    if (cor_array_init(&my->tables, pool, 16, sizeof(myslave_table_t)) != 0) {
        myslave_delete(my);
        return NULL;
    }
    /*  init trace  */
    my->trace = cor_trace_new(256);
    if (!my->trace) {
        myslave_delete(my);
        return NULL;        
    }

    return my;
}

void
myslave_delete(myslave_t *my)
{
    if (my) {
        if (my->trace) {
            cor_trace_delete(my->trace);
        }
        if (my->pool) {
            cor_pool_delete(my->pool);
        }
    }
}

const char *
myslave_get_error(myslave_t *my)
{
    return cor_trace_get(my->trace);
}

int
myslave_add_table(myslave_t *my, const char *db, const char *table)
{
    myslave_table_t *t = (myslave_table_t *) cor_array_push(&my->tables);
    if (!t) {
        cor_trace(my->trace, "can't cor_array_push");
        return -1;
    }
    memset(t, 0, sizeof(myslave_table_t));
    /*  copy db name  */
    if (myslave_copy_str(&t->db, db, strlen(db), my->pool) != 0) {
        cor_trace(my->trace, "can't myslave_copy_str");
        return -1;
    }
    /*  copy table name  */
    if (myslave_copy_str(&t->name, table, strlen(table), my->pool) != 0) {
        cor_trace(my->trace, "can't cor_pool_alloc");
        return -1;
    }
    return 0;
}

int
myslave_run(myslave_t *my)
{
    cor_mysql_t *m = cor_mysql_new(my->host.data, my->port, NULL, my->user.data, my->pwd.data);
    if (!m) {
        cor_trace(my->trace, "can't cor_mysql_new");
        return -1;
    }
    if (myslave_mysql_check(my, m) != 0) {
        cor_mysql_delete(m);
        return -1;
    }
    if (myslave_load_table_info(my, m) != 0) {
        cor_mysql_delete(m);
        return -1;
    }
    cor_mysql_delete(m);

    return 0;
}

int
myslave_mysql_check(myslave_t *my, cor_mysql_t *m)
{
    /*  check version  */
    MYSQL_RES *res = cor_mysql_query(m, "SELECT VERSION()");
    if (!res) {
        cor_trace(my->trace, "can't cor_mysql_query, error: %s", m->error);
        return -1;
    }
    char **row = cor_mysql_row(res);
    if (!row) {
        cor_trace(my->trace, "can't mysql_fetch_row, error: %s", m->error);
        cor_mysql_res_free(res);
        return -1;
    }
    if (!row[0]) {
        cor_trace(my->trace, "SELECT VERSION() didn't return value");
        cor_mysql_res_free(res);
        return -1;
    }
    int version = myslave_mysql_version_to_int(row[0]);
    if (version == -1) {
        cor_trace(my->trace, "bad mysql version format: %s", row[0]);
        cor_mysql_res_free(res);
        return -1;
    }
    cor_mysql_res_free(res);
    if (version < 5001023) {
        cor_trace(my->trace, "mysql minimal version 5.1.23, got: %s", row[0]);
        return -1;
    }
    /*  check log format  */
    res = cor_mysql_query(m, "SHOW GLOBAL VARIABLES LIKE 'binlog_format'");
    if (!res) {
        cor_trace(my->trace, "can't cor_mysql_query, error: %s", m->error);
        return -1;
    }
    row = cor_mysql_row(res);
    if (!row) {
        cor_trace(my->trace, "can't mysql_fetch_row, error: %s", m->error);
        cor_mysql_res_free(res);
        return -1;
    }
    if (!row[1]) {
        cor_trace(my->trace, "SHOW GLOBAL VARIABLES LIKE 'binlog_format' didn't return value");
        cor_mysql_res_free(res);
        return -1;
    }
    if (strcmp(row[1], "ROW") != 0) {
        cor_trace(my->trace, "mysql binlog_format ROW expected, got: %s", row[0]);
        cor_mysql_res_free(res);
        return -1;
    }
    cor_mysql_res_free(res);

    return 0;
}

int
myslave_mysql_version_to_int(const char *p)
{
    int version = 0, n = 0;
    for (; *p != '.'; p++) {
        if (*p < '0' || *p > '9') {
            return -1;
        }
        n = n * 10 + (*p - '0');
    }
    version = n * 1000000;
    n = 0;
    for (p++; *p != '.'; p++) {
        if (*p < '0' || *p > '9') {
            return -1;
        }
        n = n * 10 + (*p - '0');
    }
    version += n * 1000;
    n = 0;
    for (p++; *p != '.' && *p != '-'; p++) {
        if (*p < '0' || *p > '9') {
            return -1;
        }
        n = n * 10 + (*p - '0');
    }
    version += n;
    return version;
}

int
myslave_load_table_info(myslave_t *my, cor_mysql_t *m)
{
    myslave_table_t *tables = (myslave_table_t *) my->tables.elts;
    for (int i = 0; i < my->tables.nelts; i++) {
        myslave_table_t *t = &tables[i];
        MYSQL_RES *res = cor_mysql_query(m, "SHOW FULL COLUMNS FROM %s IN %s", t->name.data, t->db.data);
        if (!res) {
            cor_trace(my->trace, "can't cor_mysql_query, error: %s", m->error);
            return -1;
        }
        char **row;
        printf("%s.%s\n", t->db.data, t->name.data);
        while ((row = cor_mysql_row(res)) != NULL) {
            printf("  %s | %s | %s | %s | %s | %s | %s\n", row[0], row[1], row[2], row[3], row[4], row[5], row[6]);
        }
        cor_mysql_res_free(res);
    }

    return 0;
}
