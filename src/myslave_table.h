#ifndef TABLE_H
#define TABLE_H

#include "cor_mysql.h"
#include "cor_pool.h"
#include "cor_trace.h"

#include "myslave_types.h"

typedef union {
    myslave_int_t val_int;
    myslave_bigint_t val_bigint;
    myslave_mediumint_t val_mediumint;
    myslave_smallint_t val_smallint;
    myslave_tinyint_t val_tinyint;
    myslave_bit_t val_bit;
    myslave_enum_t val_enum;
    myslave_set_t val_set;
    myslave_double_t val_double;
    myslave_decimal_t val_decimal;
    myslave_float_t val_float;
    myslave_date_t val_date;
    myslave_datetime_t val_datetime;
    myslave_time_t val_time;
    myslave_timestamp_t val_timestamp;
    myslave_year_t val_year;
    myslave_char_t val_char;
    myslave_varchar_t val_varchar;
    myslave_longtext_t val_longtext;
    myslave_mediumtext_t val_mediumtext;
    myslave_text_t val_text;
    myslave_tinytext_t val_tinytext;
    myslave_blob_t val_blob;
    myslave_longblob_t val_longblob;
    myslave_mediumblob_t val_mediumblob;
    myslave_tinyblob_t val_tinyblob;
} myslave_val_t;

typedef struct
{
    myslave_str_t name;
    myslave_field_type_t type;
    myslave_str_t default_value;
    int size;
    unsigned null:1;
} myslave_field_t;

typedef struct
{
    myslave_str_t db;
    myslave_str_t name;
    /**/
    int fields_size;
    myslave_field_t *fields;
} myslave_table_t;

int myslave_table_load_info(myslave_table_t *t, cor_mysql_t *m, cor_pool_t *pool, cor_trace_t *trace);

#endif
