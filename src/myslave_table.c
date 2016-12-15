#include "myslave_table.h"

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
    char *p = strchr(row[1], '(');

}

void
myslave_table_set_field_type(myslave_field_t *f, const char *type_name)
{
    /* !!! enum  */
    int len = strlen(type_name);
    const char *p = type_name;
    const char *end = p + len;
    /*  search for size  */
    for (int i = 0; i < len; i++) {
        if (p[i] == '(') {
            end = p + i;
            for (; i < size; i++) {
                if (p[i] >= '0' && p[i] <= '9') {
                    f->size = f->size * 10 + (p[i] - '0');
                } else if (p[i] == ')') {
                    break;
                }
            }
            break;
        }
    }
    /*  determine type  */
    switch (end - p) {
        case 3:
// bit 
// int
// set
            break;
        case 4:
// blob
// char
// date 
// enum
// text 
// time 
// year 
            break;
        case 5:
// float 
            break;
        case 6:
// bigint
// double 
            break;
        case 7:
// decimal
// tinyint
// varchar
            break;
        case 8:
// datetime 
// longblob 
// longtext 
// smallint
// tinyblob 
// tinytext 
            break;
        case 9:
// mediumint
// timestamp 
            break;
        case 10:
// mediumblob 
// mediumtext 
            break;
    }

}


/*
  val_int | int(11) | (null) | NO |  | (null) | 
  val_bigint | bigint(20) | (null) | NO |  | (null) | 
  val_mediumint | mediumint(9) | (null) | NO |  | (null) | 
  val_smallint | smallint(6) | (null) | NO |  | (null) | 
  val_tinyint | tinyint(4) | (null) | NO |  | (null) | 
  val_bit | bit(1) | (null) | YES |  | (null) | 
  val_enum | enum('one','two','three') | utf8_unicode_ci | YES |  | one | 
  val_set | set('one','two') | utf8_unicode_ci | YES |  | (null) | 
  val_double | double | (null) | YES |  | (null) | 
  val_decimal | decimal(10,0) | (null) | YES |  | (null) | 
  val_float | float | (null) | YES |  | (null) | 
  val_date | date | (null) | YES |  | (null) | 
  val_datetime | datetime | (null) | YES |  | (null) | 
  val_time | time | (null) | YES |  | (null) | 
  val_timestamp | timestamp | (null) | YES |  | (null) | 
  val_year | year(4) | (null) | YES |  | (null) | 
  val_char | char(16) | utf8_unicode_ci | YES |  | (null) | 
  val_varchar | varchar(256) | utf8_unicode_ci | YES |  | (null) | 
  val_longtext | longtext | utf8_unicode_ci | YES |  | (null) | 
  val_mediumtext | mediumtext | utf8_unicode_ci | YES |  | (null) | 
  val_text | text | utf8_unicode_ci | YES |  | (null) | 
  val_tinytext | tinytext | utf8_unicode_ci | YES |  | (null) | 
  val_blob | blob | (null) | YES |  | (null) | 
  val_longblob | longblob | (null) | YES |  | (null) | 
  val_mediumblob | mediumblob | (null) | YES |  | (null) | 
  val_tinyblob | tinyblob | (null) | YES |  | (null) | 
*/
