#include "handling.h"


int Socket(int domain, int type, int protocol)
{
    int mSD = -1;
    if ((mSD = socket(domain, type, protocol)) < 0)
    {
        perror("Failed to create a socket\n");
        exit(EXIT_FAILURE);
    }
    return mSD;
}

void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{

    if((bind(sockfd, addr,addrlen)) < 0)
    {
        perror("Failed to bind sockets\n");
        exit(EXIT_FAILURE);
    }

}

void    initServerStructure(s_server *server, char *port)
{
    server->addr.sin_port = htons(atoi(port));
    server->addr.sin_family = AF_INET;
    server->addr.sin_addr.s_addr = INADDR_ANY;
    server->option = 1;
}

void    Listen(int sockfd, int backlog)
{
    if ((listen(sockfd, backlog)) < 0)
    {
        perror("Failed to listen socket\n");
        exit(EXIT_FAILURE);
    }
}

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{    
    int clientSocketFD = -1;
    if ((clientSocketFD = accept(sockfd, addr, addrlen)) < 0)
    {
        perror("Failed to accept socket\n");
        exit(EXIT_FAILURE);
    }
    return clientSocketFD;
}

ssize_t Recv(int sockfd, void *buf, size_t len, int flags)
{
    ssize_t recvVal = -1;
    if((recvVal = recv(sockfd, buf, len, flags)) < 0)
    {
        perror("Failed to receive message\n");
        exit(EXIT_FAILURE);
    }
    return recvVal;
}

void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    if ((connect(sockfd, addr, addrlen)) < 0)
    {
        perror("Failed to connect to server\n");
        exit(EXIT_FAILURE);
    }
}

void    initClientStructure(s_client *client, char *address, char *port)
{
    client->addr.sin_port = htons(atoi(port));
    client->addr.sin_family = AF_INET;
    inet_pton(AF_INET, address, &(client->addr.sin_addr.s_addr));
}

void    Send(int sockfd, const void *buf, size_t len, int flags)
{
    if((send(sockfd, buf, len, flags)) < 0)
    {
        perror("Failed to send message\n");
        exit(EXIT_FAILURE);
    }
}