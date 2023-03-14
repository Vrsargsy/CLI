#ifndef HANDLING_H
# define HANDLING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>

# define MAX_CLIENTS 5
# define BUFFER_SIZE 1024


typedef struct t_server
{
    int                 master_socket;
    struct sockaddr_in  addr;
    socklen_t           addrLen;
    int                 clients[MAX_CLIENTS];
    ssize_t             valRead[MAX_CLIENTS];
    char                buffer[BUFFER_SIZE + 1];
    int                 option;
    fd_set              readfds;
} s_server;

typedef struct t_client
{
    int sockfd;
    struct sockaddr_in addr;
    char *buffer;
    socklen_t addrLen;
} s_client;

int     Socket(int domain, int type, int protocol);
void    Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
void    initServerStructure(s_server *server, char *port);
void    initClientStructure(s_client *client, char *address, char *port);
void    Listen(int sockfd, int backlog);
int     Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
ssize_t Recv(int sockfd, void *buf, size_t len, int flags);
void    Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
void    Send(int sockfd, const void *buf, size_t len, int flags);

#endif