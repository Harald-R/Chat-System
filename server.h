#ifndef _H_SERVER 
#define _H_SERVER

#include <string.h>
#include <stdlib.h>

typedef struct ClientNode {
    int fd;
    struct ClientNode* prev;
    struct ClientNode* link;
    char ip[16];
    char username[30];
} ClientList;

ClientList *newNode(int sockfd, char* ip) {
    ClientList *np = (ClientList *)malloc( sizeof(ClientList) );
    np->fd = sockfd;
    np->prev = NULL;
    np->link = NULL;
    strncpy(np->ip, ip, 16);
    strncpy(np->username, "NULL", 5);
    return np;
}

#endif
