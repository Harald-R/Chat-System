#include "utils.h"

unsigned long hash(const char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++) != 0)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

char *clear_newline_terminator(char *str)
{
    if (str[strlen(str)-1] == '\n') {
        str[strlen(str)-1] = '\0';
    }

    return str;
}
