#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "handling.h"


int main(int ac, char **av)
{
    if (ac != 2)
    {
        perror("Invalid arguments: Arguments must be 2\n");
        exit(EXIT_FAILURE);
    }
    s_server server = {0};
    server.addrLen = (socklen_t)sizeof(server.addr);
    server.master_socket = Socket(AF_INET, SOCK_STREAM, 0); // Create socket

    int flags = fcntl(server.master_socket,F_GETFL, 0);
    flags |= O_NONBLOCK;
    fcntl(server.master_socket, F_SETFL, flags);


    setsockopt(server.master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&server.option, sizeof(server.option));
    initServerStructure(&server, av[1]);
    Bind(server.master_socket, (struct sockaddr *)&(server.addr), server.addrLen); // Bind sockets
    //fd_set
    Listen(server.master_socket, MAX_CLIENTS);
    // server.clients[clientID] = Accept(server.master_socket, (struct sockaddr *)&(server.addr), &(server.addrLen)); 
    while(1)
    {
        waitForEvents(&server);
        Recv(&server);
    }
    closeAll(&server);
}