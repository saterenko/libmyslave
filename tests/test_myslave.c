#include <string.h>
#include "cor_test.h"
#include "cor_mysql.h"

#include "../src/myslave.h"

/*  Test myslave add tables */

BEGIN_TEST(test_myslave_add_table)
{
    myslave_t *my = myslave_new("host", 0, "usr", "pwd", NULL, NULL);
    test_ptr_ne(my, NULL);
    int rc = myslave_add_table(my, "test1", "test1");
    test_int_eq(rc, 0);
    rc = myslave_add_table(my, "test1", "test2");
    test_int_eq(rc, 0);
    rc = myslave_add_table(my, "test2", "test3");
    test_int_eq(rc, 0);
    rc = myslave_add_table(my, "test2", "test4");
    test_int_eq(rc, 0);
    test_int_eq(my->tables.nelts, 4);
    myslave_table_t *t = (myslave_table_t *) my->tables.elts;
    test_str_eq(t[0].db.data, "test1");
    test_str_eq(t[0].name.data, "test1");
    test_str_eq(t[1].db.data, "test1");
    test_str_eq(t[1].name.data, "test2");
    test_str_eq(t[2].db.data, "test2");
    test_str_eq(t[2].name.data, "test3");
    test_str_eq(t[3].db.data, "test2");
    test_str_eq(t[3].name.data, "test4");
    myslave_delete(my);
}
END;

/*  Test myslave  */

static void
test_myslave_cb(const char *table, enum myslave_event_e event, myslave_field_t **fields)
{
    printf("test_myslave_cb\n");
}

BEGIN_TESTA(test_myslave(const char *host, int port, const char *db, const char *usr, const char *pwd))
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
        "val_varchar VARCHAR(256) NULL,"
        "val_longtext LONGTEXT NULL,"
        "val_mediumtext MEDIUMTEXT NULL,"
        "val_text TEXT NULL,"
        "val_tinytext TINYTEXT NULL,"
        "val_blob BLOB NULL,"
        "val_longblob LONGBLOB NULL,"
        "val_mediumblob MEDIUMBLOB NULL,"
        "val_tinyblob TINYBLOB NULL"
    ") DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci";
    cor_mysql_t *m = cor_mysql_new(host, port, db, usr, pwd);
    test_ptr_ne(m, NULL);
    int rc = cor_mysql_update(m, "drop table if exists test_myslave");
    test_int_eq(rc, 0);
    rc = cor_mysql_update(m, create_table);
    test_int_eq(rc, 0);
    cor_mysql_delete(m);
    /*  test init table  */
    myslave_t *my = myslave_new(host, port, usr, pwd, test_myslave_cb, NULL);
    test_ptr_ne(my, NULL);
    rc = myslave_add_table(my, db, "test_myslave");
    test_int_eq(rc, 0);
    rc = myslave_run(my);
    if (rc != 0) {
        printf("  error: %s\n", myslave_get_error(my));
    }
    test_int_eq(rc, 0);
    myslave_delete(my);
}
END;

int
main(int argc, char **argv)
{
    RUN_TEST(test_myslave_add_table);
    /**/
    if (argc == 5) {
        RUN_TESTA(test_myslave(argv[1], atoi(argv[2]), argv[3], argv[4], argv[5]));
    }

    exit(0);
}
