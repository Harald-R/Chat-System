#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>


#ifndef IP
#define IP "127.0.0.1"
#endif

#ifndef PORT
#define PORT 8096
#endif

#define MSG_LEN 1000

int create_socket(char *ip, int port)
{
    int sfd;
    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sfd < 0) {
        perror("Error creating socket");
        exit(1);
    }

    if(inet_pton(AF_INET, ip, &addr.sin_addr) != 1) {
        perror("Error running inet_pton()");
        exit(1);
    }

    if(connect(sfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Error connecting to socket");
        exit(1);
    }

    return sfd;
}

void* receive_messages(void *sfd){
    int socket_fd = *((int*) sfd);
    char message[MSG_LEN] = {};
    memset(message, 0, MSG_LEN);
    while (recv(socket_fd, message, MSG_LEN, 0)) {
        printf("\r%s\n", message);
    }
    return NULL;
}

int main()
{
    int socket_fd;
    char message[MSG_LEN];
    pthread_t tid;

    socket_fd = create_socket(IP, PORT);
    int status = pthread_create(&tid, NULL, receive_messages, (void *)&socket_fd);
    if (status) {
        printf ("Error creating thread");
        exit(1);
    }
    while(1) {
        printf("Enter a message: ");
        fgets(message, MSG_LEN, stdin);
        send(socket_fd, message, strlen(message), 0);
    }
}
