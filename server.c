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
    int clientID = 0;

    server.addrLen = (socklen_t)sizeof(server.addr);

    server.master_socket = Socket(AF_INET, SOCK_STREAM, 0); // Create socket

    setsockopt(server.master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&server.option, sizeof(server.option));
    initServerStructure(&server, av[1]);
    Bind(server.master_socket, (struct sockaddr *)&(server.addr), server.addrLen); // Bind sockets
    //fd_set
    Listen(server.master_socket, MAX_CLIENTS);
    server.clients[clientID] = Accept(server.master_socket, (struct sockaddr *)&(server.addr), &(server.addrLen)); 
    while(1)
    {
        write(1, "<Server>: ", 10);
        server.valRead[clientID] = Recv(server.clients[clientID], server.buffer, BUFFER_SIZE, 0);
        printf("Received msg: %s\n", server.buffer);
    }
    close(server.master_socket);

}