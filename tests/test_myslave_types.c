#include <string.h>
#include "cor_test.h"
#include "../src/myslave_types.h"

typedef struct
{
    const char *name;
    myslave_field_type_t type;
} test_myslave_types_t;

BEGIN_TEST(test_myslave_types)
{
    static test_myslave_types_t data[] = {
        {"int", MYSLAVE_INT},
        {"bigint", MYSLAVE_BIGINT},
        {"mediumint", MYSLAVE_MEDIUMINT},
        {"smallint", MYSLAVE_SMALLINT},
        {"tinyint", MYSLAVE_TINYINT},
        {"bit", MYSLAVE_BIT},
        {"enum", MYSLAVE_ENUM},
        {"set", MYSLAVE_SET},
        {"double", MYSLAVE_DOUBLE},
        {"decimal", MYSLAVE_DECIMAL},
        {"float", MYSLAVE_FLOAT},
        {"date", MYSLAVE_DATE},
        {"datetime", MYSLAVE_DATETIME},
        {"time", MYSLAVE_TIME},
        {"timestamp", MYSLAVE_TIMESTAMP},
        {"year", MYSLAVE_YEAR},
        {"char", MYSLAVE_CHAR},
        {"varchar", MYSLAVE_VARCHAR},
        {"longtext", MYSLAVE_LONGTEXT},
        {"mediumtext", MYSLAVE_MEDIUMTEXT},
        {"tinytext", MYSLAVE_TINYTEXT},
        {"text", MYSLAVE_TEXT},
        {"blob", MYSLAVE_BLOB},
        {"longblob", MYSLAVE_LONGBLOB},
        {"mediumblob", MYSLAVE_MEDIUMBLOB},
        {"tinyblob", MYSLAVE_TINYBLOB},
        {"aaaa", MYSLAVE_NONE},
        {"", MYSLAVE_NONE},
        {NULL, 0}
    };
    for (int i = 0; data[i].name; i++) {
        test_int_eq(data[i].type, myslave_type_get_by_name(data[i].name, strlen(data[i].name)));
    }
}
END;

int
main(int argc, char **argv)
{
    RUN_TEST(test_myslave_types);

    exit(0);
}
