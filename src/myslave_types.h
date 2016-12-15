#ifndef TYPES_H
#define TYPES_H

#include <inttypes.h>
#include <string.h>
#include <time.h>

#define myslave_ok  0
#define myslave_error -1

typedef enum
{
    MYSLAVE_EVENT_NONE = 0,
    MYSLAVE_EVENT_INSERT,
    MYSLAVE_EVENT_UPDATE,
    MYSLAVE_EVENT_DELETE
} myslave_event_type_t;

typedef enum
{
    MYSLAVE_NONE = 0,
    MYSLAVE_INT,
    MYSLAVE_BIGINT,
    MYSLAVE_MEDIUMINT,
    MYSLAVE_SMALLINT,
    MYSLAVE_TINYINT,
    MYSLAVE_BIT,
    MYSLAVE_ENUM,
    MYSLAVE_SET,
    MYSLAVE_DOUBLE,
    MYSLAVE_DECIMAL,
    MYSLAVE_FLOAT,
    MYSLAVE_DATE,
    MYSLAVE_DATETIME,
    MYSLAVE_TIME,
    MYSLAVE_TIMESTAMP,
    MYSLAVE_YEAR,
    MYSLAVE_CHAR,
    MYSLAVE_VARCHAR,
    MYSLAVE_LONGTEXT,
    MYSLAVE_MEDIUMTEXT,
    MYSLAVE_TINYTEXT,
    MYSLAVE_TEXT,
    MYSLAVE_BLOB,
    MYSLAVE_LONGBLOB,
    MYSLAVE_MEDIUMBLOB,
    MYSLAVE_TINYBLOB
} myslave_field_type_t;

typedef struct
{
    int size;
    char *data;
} myslave_str_t;

typedef uint32_t myslave_int_t;
typedef unsigned long long myslave_bigint_t;
typedef uint32_t myslave_mediumint_t;
typedef uint16_t myslave_smallint_t;
typedef char myslave_tinyint_t;
typedef uint64_t myslave_bit_t;
typedef int myslave_enum_t;
typedef unsigned long long myslave_set_t;
typedef double myslave_double_t;
typedef double myslave_decimal_t;
typedef float myslave_float_t;
typedef time_t myslave_date_t;
typedef time_t myslave_datetime_t;
typedef time_t myslave_time_t;
typedef time_t myslave_timestamp_t;
typedef int myslave_year_t;
typedef myslave_str_t myslave_char_t;
typedef myslave_str_t myslave_varchar_t;
typedef myslave_str_t myslave_longtext_t;
typedef myslave_str_t myslave_mediumtext_t;
typedef myslave_str_t myslave_tinytext_t;
typedef myslave_str_t myslave_text_t;
typedef myslave_str_t myslave_blob_t;
typedef myslave_str_t myslave_longblob_t;
typedef myslave_str_t myslave_mediumblob_t;
typedef myslave_str_t myslave_tinyblob_t;

myslave_field_type_t myslave_type_get_by_name(const char *name, int *size);

#endif
