#include "myslave_table.h"
#include "myslave_utils.h"

static int myslave_table_add_field(myslave_table_t *t, char **row, cor_pool_t *pool, cor_trace_t *trace);

int
myslave_table_load_info(myslave_table_t *t, cor_mysql_t *m, cor_pool_t *pool, cor_trace_t *trace)
{
    MYSQL_RES *res = cor_mysql_query(m, "SHOW FULL COLUMNS FROM %s IN %s", t->name.data, t->db.data);
    if (!res) {
        cor_trace(trace, "can't cor_mysql_query, error: %s", m->error);
        return -1;
    }
    int nfields = cor_mysql_affected_rows(m);
    if (nfields) {
        t->fields = (myslave_field_t *) cor_pool_calloc(pool, sizeof(myslave_field_t) * nfields);
        if (!t->fields) {
            cor_trace(trace, "can't cor_pool_calloc");
            return -1;
        }
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
    if (!row) {
        cor_trace(trace, "row is NULL");
        return -1;
    }
    if (!row[0]) {
        cor_trace(trace, "row[0] (Field) is NULL");
        return -1;
    }
    if (!row[1]) {
        cor_trace(trace, "row[1] (Type) is NULL");
        return -1;
    }
    if (!row[3]) {
        cor_trace(trace, "row[3] (Null) is NULL");
        return -1;
    }
    myslave_field_t *f = &t->fields[t->nfields];
    /*  column name  */
    if (myslave_copy_str(&f->name, row[0], strlen(row[0]), pool) != 0) {
        cor_trace(trace, "can't myslave_copy_str");
        return -1;
    }
    /*  column type  */
    myslave_table_set_field_type_and_size(f, row[1]);
    /*  TODO set collation row[2]  */
    
    /*  nullable  */
    f->null = strcmp(row[3], "YES") == 0;
    /*  default  */
    if (row[5]) {
        if (myslave_copy_str(&f->default_value, row[5], strlen(row[5]), pool) != 0) {
            cor_trace(trace, "can't myslave_copy_str");
            return -1;
        }
    }
    /**/
    t->nfields++;
    return 0;
}

void
myslave_table_set_field_type_and_size(myslave_field_t *f, const char *name)
{
    int len = strlen(name);
    const char *name_end = name + len;
    const char *size_begin = NULL;
    const char *size_end = NULL;
    /*  search for size  */
    for (int i = 0; i < len; i++) {
        if (name[i] == '(') {
            name_end = name + i;
            size_begin = name + i + 1;
        } else if (name[i] == ')') {
            size_end = name + i;
            break;
        }
    }
    /*  set type  */
    f->type = myslave_type_get_by_name(name, name_end - name);
    /*  set size  */
    f->size = 0;
    if (f->type != MYSLAVE_ENUM && f->type != MYSLAVE_SET && f->type != MYSLAVE_DECIMAL) {
        for (const char *p = size_begin; p < size_end; p++) {
            if (*p < '0' || *p > '9') {
                break;
            }
            f->size = f->size * 10 + (*p - '0');
        }
    }
}
