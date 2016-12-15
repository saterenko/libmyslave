#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define hash(key, c) ((unsigned int) key * 31 + c)

int
main(int argc, char **argv)
{
    static char *names[] = {"bigint", "bit", "blob", "char", "date", "datetime", "decimal", "double", "enum", "float", "int", "longblob", "longtext", "mediumblob", "mediumint", "mediumtext", "set", "smallint", "text", "time", "timestamp", "tinyblob", "tinyint", "tinytext", "varchar", "year"};
#define SIZE 64
    int table[SIZE];

    for (int i = 0; i < 25; i++) {
        const char *p = names[i];
        unsigned int h = 8;
        for (int j = 0; p[j] != '\0'; j++) {
            h = h * 37 + (unsigned char) p[j];
        }
        h = h % SIZE;
        printf("%u %s\n", h, names[i]);
    }


#if 0
    for (unsigned int b = 1; b < 1000; b++) {
        int found = 0;
        for (unsigned int n = 1; n < 100000; n++) {
            memset(&table[0], 0, sizeof(int) * SIZE);
            int i;
            for (i = 0; i < 25; i++) {
                const char *p = names[i];
                unsigned int h = n;
                for (int j = 0; p[j] != '\0'; j++) {
                    h = h * b + (unsigned char) p[j];
                }
                h = h % SIZE;
                // printf("%u, %s hash %u\n", n, names[i], h);
                if (table[h]) {
                    // printf("  %s collision with hash %u\n", names[i], h);
                    break;
                }
                table[h] = h;
            }
            if (i == 25) {
                found = 1;
                printf("\nfound, b: %u, n: %u\n", b, n);
                break;
            }
        }
        if (found) {
            break;
        }
        if (b % 10 == 0) {
            printf("\r%u", b);
        }
    }
    printf("\n");
    return 0;
#endif
}


