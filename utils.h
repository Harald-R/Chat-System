#ifndef _H_UTILS
#define _H_UTILS

#include <string.h>

#ifndef IP
#define IP "127.0.0.1"
#endif

#ifndef PORT
#define PORT 8096
#endif

#define CREDENTIALS_LEN 30
#define MSG_LEN 1000
#define MSG_LEN_EXTENDED (1000+CREDENTIALS_LEN+1)

#define MAX_CONNECTIONS 5

#define TIMEOUT 100

unsigned long hash(const char *);
char *clear_newline_terminator(char *);

#endif
