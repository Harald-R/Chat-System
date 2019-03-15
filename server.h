#ifndef LIST
#define LIST

typedef struct ClientNode {
    int fd;
    struct ClientNode* prev;
    struct ClientNode* link;
    char ip[16];
} ClientList;

ClientList *newNode(int sockfd, char* ip) {
    ClientList *np = (ClientList *)malloc( sizeof(ClientList) );
    np->fd = sockfd;
    np->prev = NULL;
    np->link = NULL;
    strncpy(np->ip, ip, 16);
    return np;
}

#endif
