#ifndef _H_UTILS
#define _H_UTILS

#include <string.h>

#ifndef IP
#define IP "127.0.0.1"
#endif

#ifndef PORT
#define PORT 8098
#endif

#define MSG_LEN 1000
#define CREDENTIALS_LEN 30

#define MAX_CONNECTIONS 5

unsigned long hash(const char *);
char *clear_newline_terminator(char *);

#endif
