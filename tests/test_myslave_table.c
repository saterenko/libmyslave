#include <string.h>
#include "cor_test.h"
#include "cor_mysql.h"

#include "../src/myslave.h"

/*  Test myslave_table_set_field_type_and_size  */

typedef struct
{
    const char *name;
    myslave_field_type_t type;
    int size;
} test_myslave_table_field_t;

BEGIN_TEST(test_myslave_table_set_field_type_and_size)
{
    static test_myslave_table_field_t data[] = {
        {"int", MYSLAVE_INT, 0},
        {"bigint", MYSLAVE_BIGINT, 0},
        {"mediumint", MYSLAVE_MEDIUMINT, 0},
        {"smallint", MYSLAVE_SMALLINT, 0},
        {"tinyint", MYSLAVE_TINYINT, 0},
        {"bit", MYSLAVE_BIT, 0},
        {"enum", MYSLAVE_ENUM, 0},
        {"set", MYSLAVE_SET, 0},
        {"double", MYSLAVE_DOUBLE, 0},
        {"decimal", MYSLAVE_DECIMAL, 0},
        {"float", MYSLAVE_FLOAT, 0},
        {"date", MYSLAVE_DATE, 0},
        {"datetime", MYSLAVE_DATETIME, 0},
        {"time", MYSLAVE_TIME, 0},
        {"timestamp", MYSLAVE_TIMESTAMP, 0},
        {"year", MYSLAVE_YEAR, 0},
        {"char", MYSLAVE_CHAR, 0},
        {"varchar", MYSLAVE_VARCHAR, 0},
        {"longtext", MYSLAVE_LONGTEXT, 0},
        {"mediumtext", MYSLAVE_MEDIUMTEXT, 0},
        {"tinytext", MYSLAVE_TINYTEXT, 0},
        {"text", MYSLAVE_TEXT, 0},
        {"blob", MYSLAVE_BLOB, 0},
        {"longblob", MYSLAVE_LONGBLOB, 0},
        {"mediumblob", MYSLAVE_MEDIUMBLOB, 0},
        {"tinyblob", MYSLAVE_TINYBLOB, 0},
        /*  with size */
        {"int(11)", MYSLAVE_INT, 11},
        {"bigint(20)", MYSLAVE_BIGINT, 20},
        {"mediumint(9)", MYSLAVE_MEDIUMINT, 9},
        {"smallint(6)", MYSLAVE_SMALLINT, 6},
        {"tinyint(4)", MYSLAVE_TINYINT, 4},
        {"bit(1)", MYSLAVE_BIT, 1},
        {"enum('one','two','three')", MYSLAVE_ENUM, 0},
        {"set('one','two')", MYSLAVE_SET, 0},
        {"double", MYSLAVE_DOUBLE, 0},
        {"decimal", MYSLAVE_DECIMAL, 0},
        {"float", MYSLAVE_FLOAT, 0},
        {"date", MYSLAVE_DATE, 0},
        {"datetime", MYSLAVE_DATETIME, 0},
        {"time", MYSLAVE_TIME, 0},
        {"timestamp", MYSLAVE_TIMESTAMP, 0},
        {"year(4)", MYSLAVE_YEAR, 4},
        {"char(16)", MYSLAVE_CHAR, 16},
        {"varchar(255)", MYSLAVE_VARCHAR, 255},
        {"longtext", MYSLAVE_LONGTEXT, 0},
        {"mediumtext", MYSLAVE_MEDIUMTEXT, 0},
        {"tinytext", MYSLAVE_TINYTEXT, 0},
        {"text", MYSLAVE_TEXT, 0},
        {"blob", MYSLAVE_BLOB, 0},
        {"longblob", MYSLAVE_LONGBLOB, 0},
        {"mediumblob", MYSLAVE_MEDIUMBLOB, 0},
        {"tinyblob", MYSLAVE_TINYBLOB, 0},
        {"aaaa", MYSLAVE_NONE, 0},
        {"", MYSLAVE_NONE, 0},
        {NULL, 0, 0}
    };
    for (int i = 0; data[i].name; i++) {
        myslave_field_t f;
        myslave_table_set_field_type_and_size(&f, data[i].name);
        test_int_eq(f.type, data[i].type);
        test_int_eq(f.size, data[i].size);
    }
}
END;

/*  Test myslave table info */

static void
test_myslave_table_cb(const char *table, myslave_event_type_t event, myslave_field_t **fields)
{
    printf("test_myslave_table_cb\n");
}

BEGIN_TESTA(test_myslave_table_load_info(const char *host, int port, const char *db, const char *usr, const char *pwd))
{
    /*  init table  */
    static char *create_table = "CREATE TABLE test_myslave ("
        "val_int INT NOT NULL,"
        "val_bigint BIGINT NOT NULL,"
        "val_mediumint MEDIUMINT NOT NULL,"
        "val_smallint SMALLINT NOT NULL,"
        "val_tinyint TINYINT NOT NULL,"
        "val_bit BIT NULL,"
        "val_enum ENUM('one','two','three') NULL DEFAULT 'one',"
        "val_set SET('one','two') NULL,"
        "val_double DOUBLE NULL,"
        "val_decimal DECIMAL NULL,"
        "val_float FLOAT NULL,"
        "val_date DATE NULL,"
        "val_datetime DATETIME NULL,"
        "val_time TIME NULL,"
        "val_timestamp TIMESTAMP NULL,"
        "val_year YEAR NULL,"
        "val_char CHAR(16) NULL,"
        "val_varchar VARCHAR(255) NULL,"
        "val_longtext LONGTEXT NULL,"
        "val_mediumtext MEDIUMTEXT NULL,"
        "val_text TEXT NULL,"
        "val_tinytext TINYTEXT NULL,"
        "val_blob BLOB NULL,"
        "val_longblob LONGBLOB NULL,"
        "val_mediumblob MEDIUMBLOB NULL,"
        "val_tinyblob TINYBLOB NULL"
    ") DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci";
    /**/
    static myslave_field_t table_info[] = {
        {myslave_str_init("val_int"), MYSLAVE_INT, myslave_str_null, 11, 0},
        {myslave_str_init("val_bigint"), MYSLAVE_BIGINT, myslave_str_null, 20, 0},
        {myslave_str_init("val_mediumint"), MYSLAVE_MEDIUMINT, myslave_str_null, 9, 0},
        {myslave_str_init("val_smallint"), MYSLAVE_SMALLINT, myslave_str_null, 6, 0},
        {myslave_str_init("val_tinyint"), MYSLAVE_TINYINT, myslave_str_null, 4, 0},
        {myslave_str_init("val_bit"), MYSLAVE_BIT, myslave_str_null, 1, 1},
        {myslave_str_init("val_enum"), MYSLAVE_ENUM, myslave_str_init("one"), 0, 1},
        {myslave_str_init("val_set"), MYSLAVE_SET, myslave_str_null, 0, 1},
        {myslave_str_init("val_double"), MYSLAVE_DOUBLE, myslave_str_null, 0, 1},
        {myslave_str_init("val_decimal"), MYSLAVE_DECIMAL, myslave_str_null, 0, 1},
        {myslave_str_init("val_float"), MYSLAVE_FLOAT, myslave_str_null, 0, 1},
        {myslave_str_init("val_date"), MYSLAVE_DATE, myslave_str_null, 0, 1},
        {myslave_str_init("val_datetime"), MYSLAVE_DATETIME, myslave_str_null, 0, 1},
        {myslave_str_init("val_time"), MYSLAVE_TIME, myslave_str_null, 0, 1},
        {myslave_str_init("val_timestamp"), MYSLAVE_TIMESTAMP, myslave_str_null, 0, 1},
        {myslave_str_init("val_year"), MYSLAVE_YEAR, myslave_str_null, 4, 1},
        {myslave_str_init("val_char"), MYSLAVE_CHAR, myslave_str_null, 16, 1},
        {myslave_str_init("val_varchar"), MYSLAVE_VARCHAR, myslave_str_null, 255, 1},
        {myslave_str_init("val_longtext"), MYSLAVE_LONGTEXT, myslave_str_null, 0, 1},
        {myslave_str_init("val_mediumtext"), MYSLAVE_MEDIUMTEXT, myslave_str_null, 0, 1},
        {myslave_str_init("val_text"), MYSLAVE_TEXT, myslave_str_null, 0, 1},
        {myslave_str_init("val_tinytext"), MYSLAVE_TINYTEXT, myslave_str_null, 0, 1},
        {myslave_str_init("val_blob"), MYSLAVE_BLOB, myslave_str_null, 0, 1},
        {myslave_str_init("val_longblob"), MYSLAVE_LONGBLOB, myslave_str_null, 0, 1},
        {myslave_str_init("val_mediumblob"), MYSLAVE_MEDIUMBLOB, myslave_str_null, 0, 1},
        {myslave_str_init("val_tinyblob"), MYSLAVE_TINYBLOB, myslave_str_null, 0, 1}
    };
    /**/
    cor_mysql_t *m = cor_mysql_new(host, port, db, usr, pwd);
    test_ptr_ne(m, NULL);
    int rc = cor_mysql_update(m, "drop table if exists test_myslave");
    test_int_eq(rc, 0);
    rc = cor_mysql_update(m, create_table);
    test_int_eq(rc, 0);
    /*  test load table info  */
    myslave_t *my = myslave_new(host, port, usr, pwd, test_myslave_table_cb, NULL);
    test_ptr_ne(my, NULL);
    rc = myslave_add_table(my, db, "test_myslave");
    test_int_eq(rc, 0);
    test_int_eq(my->tables.nelts, 1);
    myslave_table_t *tables = (myslave_table_t *) my->tables.elts;
    test_ptr_ne(tables, NULL);
    myslave_table_t *t = &tables[0];
    test_ptr_ne(t, NULL);
    rc = myslave_table_load_info(t, m, my->pool, my->trace);
    // if (rc != 0) {
    //     printf("  error: %s\n", myslave_get_error(my));
    // }
    test_int_eq(rc, 0);
    test_int_eq(t->nfields, 26);
    for (int i = 0; i < t->nfields; i++) {
        myslave_field_t *f = &t->fields[i];
        test_ptr_ne(f, NULL);
        myslave_field_t *f2 = &table_info[i];
        test_int_eq(f->name.size, f2->name.size);
        if (f->name.size) {
            test_str_eq(f->name.data, f2->name.data);
        } else {
            test_ptr_eq(f->name.data, NULL);
        }
        test_int_eq(f->type, f2->type);
        test_int_eq(f->default_value.size, f2->default_value.size);
        if (f->default_value.size) {
            test_str_eq(f->default_value.data, f2->default_value.data);
        } else {
            test_ptr_eq(f->default_value.data, NULL);
        }
        test_int_eq(f->size, f2->size);
        test_int_eq(f->null, f2->null);
    }
    myslave_delete(my);
    cor_mysql_delete(m);
}
END;

int
main(int argc, char **argv)
{
    RUN_TEST(test_myslave_table_set_field_type_and_size);
    if (argc == 5) {
        RUN_TESTA(test_myslave_table_load_info(argv[1], atoi(argv[2]), argv[3], argv[4], argv[5]));
    }

    exit(0);
}
