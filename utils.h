#ifndef _H_UTILS
#define _H_UTILS

#include <string.h>

#ifndef IP
#define IP "127.0.0.1"
#endif

#ifndef PORT
#define PORT 8096
#endif

#define MSG_LEN 1000
#define CREDENTIALS_LEN 30

#define MAX_CONNECTIONS 5

#define TIMEOUT 100

unsigned long hash(const char *);
char *clear_newline_terminator(char *);

#endif
