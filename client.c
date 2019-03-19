#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include "utils.h"

const char *user_prompt = "> ";

void print_prompt()
{
    printf("\r%s ", user_prompt);
    fflush(stdout);
}

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

void *receive_messages(void *sfd){
    int socket_fd = *((int*) sfd);
    char message[MSG_LEN_EXTENDED];
    memset(message, 0, MSG_LEN_EXTENDED);

    char msg_sender[30];
    int n;

    while (1) {
        n = recv(socket_fd, message, MSG_LEN_EXTENDED, 0);
        if (n == 0) {
            printf("Lost connection to server\n");
            close(socket_fd);
            exit(1);
        }

        // Check if the message is a reply from the server
        if (strncmp(message, "SRV:", 4) == 0) {
            if (strstr(message, "LOGIN_FAIL") != NULL) {
                printf("\rError authenticating\n");
                exit(1);
            } else if (strstr(message, "LOGIN_SUCCESS") != NULL) {
                printf("\rSuccessful autentication!\n");
                print_prompt();
            } else if (strstr(message, "ERROR") != NULL) {
                printf("\rError authenticating\n");
                //printf("Error occurred\n");
                exit(1);
            }
        } else {
            sscanf(message, "%[^:]", msg_sender);
            char *message_content = message + (strlen(msg_sender) + 1);
            printf("\r%s: %s\n", msg_sender, message_content);
            print_prompt();
            memset(msg_sender, 0, sizeof(msg_sender));
        }
    }
    return NULL;
}

int main()
{
    int socket_fd;
    char message[MSG_LEN];
    char username[CREDENTIALS_LEN];
    char password[CREDENTIALS_LEN];
    pthread_t tid;

    memset(username, 0, CREDENTIALS_LEN);
    memset(password, 0, CREDENTIALS_LEN);

    // Prompt client for credentials
    printf("Please enter your username: ");
    fgets(username, CREDENTIALS_LEN, stdin);
    if (strlen(username) > CREDENTIALS_LEN) {
        printf("\nUsername cannot have more than thirty characters.\n");
        exit(1);
    }

    printf("Please enter your password: ");
    fgets(password, CREDENTIALS_LEN, stdin);
    if (strlen(password) > CREDENTIALS_LEN) {
        printf("\nPassword cannot have more than thirty characters.\n");
        exit(1);
    }

    strcpy(password, clear_newline_terminator(password));
    unsigned long passHashed = hash(password);
    sprintf(password, "%lu", passHashed);

    // Create socket and thread for receiving messages
    socket_fd = create_socket(IP, PORT);
    int status = pthread_create(&tid, NULL, receive_messages, (void *)&socket_fd);
    if (status) {
        printf ("Error creating thread");
        exit(1);
    }

    // Send username and password
    strcpy(username, clear_newline_terminator(username));
    strcpy(password, clear_newline_terminator(password));

    sprintf(message, "%s:%s:%s", "login", username, password);
    if (send(socket_fd, message, strlen(message), 0) < 0) {
        perror("Error sending credentials");
        exit(1);
    }

    // Allow user to send messages
    while (1) {
        print_prompt();
        fgets(message, MSG_LEN, stdin);
        strcpy(message, clear_newline_terminator(message));
    
        send(socket_fd, message, strlen(message), 0);
    }
}
