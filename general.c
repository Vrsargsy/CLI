#include "general.h"

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

void    Send(int sockfd, const void *buf, size_t len, int flags)
{
    if((send(sockfd, buf, len, flags)) < 0)
    {
        perror("Failed to send message\n");
        exit(EXIT_FAILURE);
    }
}