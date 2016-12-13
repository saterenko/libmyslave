#ifndef TABLE_H
#define TABLE_H

#include "myslave_types.h"

typedef union {
    myslave_int_t val_int;
    myslave_bigint_t val_bigint;
    myslave_mediumint_t val_mediumint;
    myslave_tinyint_t val_tinyint;
    myslave_bit_t val_bit;
    myslave_enum_t val_enum;
    myslave_set_t val_set;
    myslave_double_t val_double;
    myslave_decimal_t val_decimal;
    myslave_datetime_t val_datetime;
    myslave_timestamp_t val_timestamp;
    myslave_char_t val_char;
    myslave_varchar_t val_varchar;
    myslave_tinytext_t val_tinytext;
    myslave_text_t val_text;
    myslave_blob_t val_blob;
} myslave_val_t;

typedef struct
{
    myslave_str_t name;
    myslave_val_t old;
    myslave_val_t new;
} myslave_field_t;

typedef struct
{
    myslave_str_t db;
    myslave_str_t name;
} myslave_table_t;


#endif
