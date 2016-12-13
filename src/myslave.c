#include "myslave.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mysql.h>
#include <my_global.h>
#include <m_ctype.h>
#include <sql_common.h>

#include "cor_array.h"

#define MYSLAVE_POOL_SIZE 8 * 1024
#define MYSLAVE_ERROR_SIZE 1024

struct myslave_s
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
    char error[MYSLAVE_ERROR_SIZE + 1];
};

int myslave_mysql_check(myslave_t *my);
int myslave_mysql_version_to_int(const char *p);

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
    size_t len = strlen(host);
    my->host = (char *) cor_pool_alloc(pool, len + 1);
    if (!my->host) {
        cor_pool_delete(pool);
        return NULL;
    }
    memcpy(my->host, host, len + 1);
    /*  copy user  */
    len = strlen(user);
    my->user = (char *) cor_pool_alloc(pool, len + 1);
    if (!my->user) {
        cor_pool_delete(pool);
        return NULL;
    }
    memcpy(my->user, user, len + 1);
    /*  copy password  */
    len = strlen(pwd);
    my->pwd = (char *) cor_pool_alloc(pool, len + 1);
    if (!my->pwd) {
        cor_pool_delete(pool);
        return NULL;
    }
    memcpy(my->pwd, pwd, len + 1);
    /*  init tables  */
    if (cor_array_init(&my->tables, pool, 16, sizeof(myslave_table_t)) != 0) {
        cor_pool_delete(pool);
        return NULL;
    }

    return my;
}

const char *
myslave_get_error(myslave_t *my)
{
    return (const char *) my->error;
}

void
myslave_delete(myslave_t *my)
{
    if (my) {
        if (my->pool) {
            cor_pool_delete(my->pool);
        }
    }
}

int
myslave_add_table(myslave_t *my, const char *db, const char *table)
{
    myslave_table_t *t = (myslave_table_t *) cor_array_push(&my->tables);
    if (!t) {
        snprintf(my->error, MYSLAVE_ERROR_SIZE, "can't cor_array_push");
        return -1;
    }
    memset(t, 0, sizeof(myslave_table_t));
    /*  copy db name  */
    t->db.size = strlen(db);
    t->db.data = (char *) cor_pool_alloc(my->pool, t->db.size + 1);
    if (!t->db.data) {
        snprintf(my->error, MYSLAVE_ERROR_SIZE, "can't cor_pool_alloc");
        return -1;
    }
    memcpy(t->db.data, db, t->db.size + 1);
    /*  copy table name  */
    t->name.size = strlen(table);
    t->name.data = (char *) cor_pool_alloc(my->pool, t->name.size + 1);
    if (!t->name.data) {
        snprintf(my->error, MYSLAVE_ERROR_SIZE, "can't cor_pool_alloc");
        return -1;
    }
    memcpy(t->name.data, table, t->name.size + 1);
    return 0;
}

int
myslave_run(myslave_t *my)
{
    if (myslave_mysql_check(my) != 0) {
        return -1;
    }

    return 0;
}

int
myslave_mysql_check(myslave_t *my)
{
    MYSQL *mysql = mysql_init(NULL);
    if (!mysql) {
        snprintf(my->error, MYSLAVE_ERROR_SIZE, "can't mysql_init");
        return -1;
    }
    if (!mysql_real_connect(mysql, my->host, my->user, my->pwd, NULL, my->port, NULL, 0)) {
        snprintf(my->error, MYSLAVE_ERROR_SIZE, "can't mysql_real_connect");
        mysql_close(mysql);
        return -1;
    }
    /*  check version  */
    if (mysql_real_query(mysql, "SELECT VERSION()", sizeof("SELECT VERSION()") - 1)) {
        snprintf(my->error, MYSLAVE_ERROR_SIZE, "can't mysql_real_query, error: %s (%d)", mysql_error(mysql), mysql_errno(mysql));
        mysql_close(mysql);
        return -1;
    }
    MYSQL_RES *res = mysql_store_result(mysql);
    if (!res) {
        snprintf(my->error, MYSLAVE_ERROR_SIZE, "can't mysql_store_result, error: %s (%d)", mysql_error(mysql), mysql_errno(mysql));
        mysql_close(mysql);
        return -1;
    }
    char **row = mysql_fetch_row(res);
    if (!row) {
        snprintf(my->error, MYSLAVE_ERROR_SIZE, "can't mysql_fetch_row, error: %s (%d)", mysql_error(mysql), mysql_errno(mysql));
        mysql_free_result(res);
        mysql_close(mysql);
        return -1;
    }
    if (!row[0]) {
        snprintf(my->error, MYSLAVE_ERROR_SIZE, "SELECT VERSION() didn't return value");
        mysql_free_result(res);
        mysql_close(mysql);
        return -1;
    }
    int version = myslave_mysql_version_to_int(row[0]);
    if (version == -1) {
        snprintf(my->error, MYSLAVE_ERROR_SIZE, "bad mysql version format: %s", row[0]);
        mysql_free_result(res);
        mysql_close(mysql);
        return -1;
    }
    mysql_free_result(res);
    if (version < 5001023) {
        snprintf(my->error, MYSLAVE_ERROR_SIZE, "mysql minimal version 5.1.23, got: %s", row[0]);
        mysql_close(mysql);
        return -1;
    }
    /*  check log format  */
    if (mysql_real_query(mysql, "SHOW GLOBAL VARIABLES LIKE 'binlog_format'", sizeof("SHOW GLOBAL VARIABLES LIKE 'binlog_format'") - 1)) {
        snprintf(my->error, MYSLAVE_ERROR_SIZE, "can't mysql_real_query, error: %s (%d)", mysql_error(mysql), mysql_errno(mysql));
        mysql_close(mysql);
        return -1;
    }
    res = mysql_store_result(mysql);
    if (!res) {
        snprintf(my->error, MYSLAVE_ERROR_SIZE, "can't mysql_store_result, error: %s (%d)", mysql_error(mysql), mysql_errno(mysql));
        mysql_close(mysql);
        return -1;
    }
    row = mysql_fetch_row(res);
    if (!row) {
        snprintf(my->error, MYSLAVE_ERROR_SIZE, "can't mysql_fetch_row, error: %s (%d)", mysql_error(mysql), mysql_errno(mysql));
        mysql_free_result(res);
        mysql_close(mysql);
        return -1;
    }
    if (!row[0]) {
        snprintf(my->error, MYSLAVE_ERROR_SIZE, "SHOW GLOBAL VARIABLES LIKE 'binlog_format' didn't return value");
        mysql_free_result(res);
        mysql_close(mysql);
        return -1;
    }
    if (strcmp(row[0], "ROW") != 0) {
        snprintf(my->error, MYSLAVE_ERROR_SIZE, "mysql binlog_format ROW expected, got: %s", row[0]);
        mysql_free_result(res);
        mysql_close(mysql);
        return -1;
    }
    mysql_free_result(res);
    mysql_close(mysql);

    return 0;
}

int
myslave_mysql_version_to_int(const char *p)
{
    int version = 0, n = 0;
    while (*p != '.') {
        if (*p < '0' || *p > '9') {
            return -1;
        }
        n = n * (*p - '0');
        p++;
    }
    version = n * 1000000;
    n = 0;
    while (*p != '.') {
        if (*p < '0' || *p > '9') {
            return -1;
        }
        n = n * (*p - '0');
        p++;
    }
    version += n * 1000;
    n = 0;
    while (*p != '.') {
        if (*p < '0' || *p > '9') {
            return -1;
        }
        n = n * (*p - '0');
        p++;
    }
    version += n;

    return version;
}


