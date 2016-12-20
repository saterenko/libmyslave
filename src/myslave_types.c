#include "myslave_types.h"

myslave_field_type_t
myslave_type_get_by_name(const char *name, int size)
{
    switch (size) {
        case 3:
            switch (name[0]) {
                case 'b':
                    return strncmp(&name[1], "it", 2) == 0 ? MYSLAVE_BIT : MYSLAVE_NONE;
                case 'i':
                    return strncmp(&name[1], "nt", 2) == 0 ? MYSLAVE_INT : MYSLAVE_NONE;
                case 's':
                    return strncmp(&name[1], "et", 2) == 0 ? MYSLAVE_SET : MYSLAVE_NONE;
            }
            break;
        case 4:
            switch (name[0]) {
                case 'b':
                    return strncmp(&name[1], "lob", 3) == 0 ? MYSLAVE_BLOB : MYSLAVE_NONE;
                case 'c':
                    return strncmp(&name[1], "har", 3) == 0 ? MYSLAVE_CHAR : MYSLAVE_NONE;
                case 'd':
                    return strncmp(&name[1], "ate", 3) == 0 ? MYSLAVE_DATE : MYSLAVE_NONE;
                case 'e':
                    return strncmp(&name[1], "num", 3) == 0 ? MYSLAVE_ENUM : MYSLAVE_NONE;
                case 't':
                    switch (name[1]) {
                        case 'e':
                            return strncmp(&name[2], "xt", 2) == 0 ? MYSLAVE_TEXT : MYSLAVE_NONE;
                        case 'i':
                            return strncmp(&name[2], "me", 2) == 0 ? MYSLAVE_TIME : MYSLAVE_NONE;
                    }
                    break;
                case 'y':
                    return strncmp(&name[1], "ear", 3) == 0 ? MYSLAVE_YEAR : MYSLAVE_NONE;
            }
            break;
        case 5:
            return strncmp(name, "float", 5) == 0 ? MYSLAVE_FLOAT : MYSLAVE_NONE;
        case 6:
            switch (name[0]) {
                case 'b':
                    return strncmp(&name[1], "igint", 5) == 0 ? MYSLAVE_BIGINT : MYSLAVE_NONE;
                case 'd':
                    return strncmp(&name[1], "ouble", 5) == 0 ? MYSLAVE_DOUBLE : MYSLAVE_NONE;
            }
            break;
        case 7:
            switch (name[0]) {
                case 'd':
                    return strncmp(&name[1], "ecimal", 6) == 0 ? MYSLAVE_DECIMAL : MYSLAVE_NONE;
                case 't':
                    return strncmp(&name[1], "inyint", 6) == 0 ? MYSLAVE_TINYINT : MYSLAVE_NONE;
                case 'v':
                    return strncmp(&name[1], "archar", 6) == 0 ? MYSLAVE_VARCHAR : MYSLAVE_NONE;
            }
            break;
        case 8:
            switch (name[0]) {
                case 'd':
                    return strncmp(&name[1], "atetime", 7) == 0 ? MYSLAVE_DATETIME : MYSLAVE_NONE;
                case 'l':
                    if (strncmp(&name[1], "ong", 3) == 0) {
                        switch (name[4]) {
                            case 'b':
                                return strncmp(&name[5], "lob", 3) == 0 ? MYSLAVE_LONGBLOB : MYSLAVE_NONE;
                            case 't':
                                return strncmp(&name[5], "ext", 3) == 0 ? MYSLAVE_LONGTEXT : MYSLAVE_NONE;
                        }
                    }
                    break;
                case 's':
                    return strncmp(&name[1], "mallint", 7) == 0 ? MYSLAVE_SMALLINT : MYSLAVE_NONE;
                case 't':
                    if (strncmp(&name[1], "iny", 3) == 0) {
                        switch (name[4]) {
                            case 'b':
                                return strncmp(&name[5], "lob", 3) == 0 ? MYSLAVE_TINYBLOB : MYSLAVE_NONE;
                            case 't':
                                return strncmp(&name[5], "ext", 3) == 0 ? MYSLAVE_TINYTEXT : MYSLAVE_NONE;
                        }
                    }
                    break;
            }
            break;
        case 9:
            switch (name[0]) {
                case 'm':
                    return strncmp(&name[1], "ediumint", 8) == 0 ? MYSLAVE_MEDIUMINT : MYSLAVE_NONE;
                case 't':
                    return strncmp(&name[1], "imestamp", 8) == 0 ? MYSLAVE_TIMESTAMP : MYSLAVE_NONE;
            }
            break;
        case 10:
            if (strncmp(name, "medium", 6) == 0) {
                switch (name[6]) {
                    case 'b':
                        return strncmp(&name[7], "lob", 3) == 0 ? MYSLAVE_MEDIUMBLOB : MYSLAVE_NONE;
                    case 't':
                        return strncmp(&name[7], "ext", 3) == 0 ? MYSLAVE_MEDIUMTEXT : MYSLAVE_NONE;
                }
            }
            break;
    }
    return MYSLAVE_NONE;
}
