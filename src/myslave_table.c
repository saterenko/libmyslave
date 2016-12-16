#include "myslave_table.h"

static int myslave_table_add_field(myslave_table_t *t, char **row, cor_pool_t *pool, cor_trace_t *trace);
static void myslave_table_set_field_type(myslave_field_t *f, const char *name);

int
myslave_table_load_info(myslave_table_t *t, cor_mysql_t *m, cor_pool_t *pool, cor_trace_t *trace)
{
    MYSQL_RES *res = cor_mysql_query(m, "SHOW FULL COLUMNS FROM %s IN %s", t->name.data, t->db.data);
    if (!res) {
        cor_trace(trace, "can't cor_mysql_query, error: %s", m->error);
        return -1;
    }
    char **row;
    while ((row = cor_mysql_row(res)) != NULL) {
        if (myslave_table_add_field(t, row, pool, trace) != 0) {
            cor_mysql_res_free(res);
            return -1;
        }
    }
    cor_mysql_res_free(res);
    return 0;
}

int
myslave_table_add_field(myslave_table_t *t, char **row, cor_pool_t *pool, cor_trace_t *trace)
{
    if (!row || !row[0] || !row[1] || !row[2] || !row[3] || !row[4]) {
        return -1;
    }
    myslave_field_t *f = (myslave_field_t *) cor_pool_calloc(pool, sizeof(myslave_field_t));
    if (!f) {
        cor_trace(trace, "can't cor_pool_calloc");
        return -1;
    }
    /*  column name  */
    f->name.size = strlen(row[0]);
    f->name.data = (char *) cor_pool_alloc(pool, f->name.size + 1);
    if (!f->name.data) {
        cor_trace(trace, "can't cor_pool_alloc");
        return -1;
    }
    memcpy(f->name.data, row[0], f->name.size + 1);
    /*  column type  */
    myslave_table_set_field_type(f, row[1]);

    return 0;
}

void
myslave_table_set_field_type(myslave_field_t *f, const char *name)
{
    int len = strlen(name);
    const char *name_end = name + len;
    const char *size_begin = NULL;
    const char *size_end = NULL;
    /*  search for size  */
    for (int i = 0; i < len; i++) {
        if (name[i] == '(') {
            name_end = &name[i] + i;
            size_begin = &name[i] + 1;
        } else if (name[i] == ')') {
            size_end = &name[i];
        }
    }
    /*  set type  */
    f->type = myslave_type_get_by_name(name, name_end - name);
    /*  set size  */
    if (f->type == MYSLAVE_ENUM || f->type == MYSLAVE_SET) {
        /*  TODO  */
        return;
    } else {
        for (const char *p = size_begin; p < size_end; p++) {
            if (*p < '0' || *p > '9') {
                break;
            }
            f->size = f->size * 10 + (*p - '0');
        }
    }
}
