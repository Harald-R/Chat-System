#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#ifndef PORT
#define PORT 8096
#endif

#define MSG_LEN 1000
#define MAX_CONNECTIONS 5

int create_socket(int port)
{
    int sfd;
    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sfd < 0) {
        perror("Error creating socket");
        exit(1);
    }

    if(bind(sfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Error binding");
        exit(1);
    }

    if(listen(sfd, MAX_CONNECTIONS) < 0) {
        perror("Error listening");
        exit(1);
    }

    return sfd;
}

void *receive_messages(void *sfd)
{
    int socket_fd = *((int*) sfd);
    char message[MSG_LEN];

    memset(message, 0, MSG_LEN);

    while(recv(socket_fd, message, MSG_LEN, 0) > 0) {
        printf("Message Received: %s\n", message);
        strcpy(message, "");
    }

    return NULL;
}

int main()
{
    struct sockaddr_in addr;
    unsigned int socket_len;
    int socket_fd;
    int client;
    pthread_t tid;

    socket_fd = create_socket(PORT);

    socket_len = sizeof(struct sockaddr_in);
    while((client = accept(socket_fd, (struct sockaddr*)&addr, &socket_len))) {
        if(client < 0) {
            perror("Error accepting connection");
            exit(1);
        }

        int status = pthread_create(&tid, NULL, receive_messages, (void *)&client);
        if(status) {
            perror("Error creating thread");
            exit(1);
        }
    }

    return 0;
}
