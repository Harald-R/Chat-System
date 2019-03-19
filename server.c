#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "server.h"
#include "authentication.h"
#include "utils.h"

ClientList *root, *last;

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

void send_to_all_clients(ClientList *np, char tmp_buffer[]) {
    ClientList *tmp = root->link;
    while (tmp != NULL) {
        if (np->fd != tmp->fd) { 
            printf("Send to sockfd %d: \"%s\" \n", tmp->fd, tmp_buffer);
            send(tmp->fd, tmp_buffer, MSG_LEN, 0);
        }
        tmp = tmp->link;
    }
}

void *client_handler(void *client)
{
    int fd = *((int*) client);
    char message[MSG_LEN];
    char message_with_username[MSG_LEN + 30];

    memset(message, 0, MSG_LEN);
    ClientList *np = (ClientList *)client;

    while (recv(fd, message, MSG_LEN, 0) > 0) {
        printf("Message Received: %s\n", message);

        enum MsgType msg_type = check_msg_type(message);
        if (msg_type != REGULAR)
        {
            char type[30], username[30], password[30];
            if (sscanf(message, "%[^:]:%[^:]:%[^:]", type, username, password) != 3) {
                strcpy(message, "An error occurred\0");
                send(fd, message, MSG_LEN, 0);
                
                perror("Error extracting information from message");
                return NULL;
            }

            if (!authenticate(username, password)) {
                strcpy(message, "SRV:LOGIN_FAIL");
                send(fd, message, MSG_LEN, 0);
                return NULL;
            }

            // Login successful
            strcpy(np->username, username);
            strcpy(message, "SRV:LOGIN_SUCCESS");
            send(fd, message, MSG_LEN, 0);
            memset(message, 0, MSG_LEN);
        }
        else
        {
            sprintf(message_with_username, "%s:%s", np->username, message);
            send_to_all_clients(np, message_with_username);
            memset(message, 0, MSG_LEN);
        }
    }

    return NULL;
}

int main()
{
    struct sockaddr_in client_addr, server_addr;
    unsigned int socket_len;
    int server_fd;
    int client_fd;
    pthread_t tid;

    server_fd = create_socket(PORT);
    socket_len = sizeof(struct sockaddr_in);

    getsockname(server_fd, (struct sockaddr*) &server_addr, (socklen_t*) &socket_len);
    printf("Start Server on: %s:%d\n", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));

    root = newNode(server_fd, inet_ntoa(server_addr.sin_addr));
    last = root;

    while((client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &socket_len))) {
        if(client_fd < 0) {
            perror("Error accepting connection");
            exit(1);
        }

        ClientList *c = newNode(client_fd, inet_ntoa(client_addr.sin_addr));
        c->prev = last;
        last->link = c;
        last = c;

        int status = pthread_create(&tid, NULL, client_handler, (void *)c);
        if(status) {
            perror("Error creating thread");
            exit(1);
        }
    }

    return 0;
}
