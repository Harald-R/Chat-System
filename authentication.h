#ifndef _H_AUTHENTICATION
#define _H_AUTHENTICATION

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define DB_FILE "data.txt"

enum MsgType {
    ERROR,
    REGULAR,
    REGISTER,
    LOGIN
};

bool authenticate(const char *, const char *);
bool registration(const char *, const char *);
enum MsgType check_msg_type(const char *);

#endif
