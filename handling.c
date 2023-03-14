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
    server->poll_fds[0].fd = server->master_socket;
    server->poll_fds[0].events = POLLIN | POLLOUT;
    server->poll_fds[0].revents = 0;
    server->clientID = 0;
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

void Recv(s_server *server)
{
    // ssize_t recvVal = -1;
    // if((recvVal = recv(sockfd, buf, len, flags)) < 0)
    // {
    //     perror("Failed to receive message\n");
    //     exit(EXIT_FAILURE);
    // }
    // return recvVal;

    for(int i = 0; i < server->clientID; i++)
    {
        if(server->poll_fds[i+1].revents & POLLIN)
        {
            ssize_t recvVal = recv(server->clients[i], server->buffer, BUFFER_SIZE, 0);
            if(recvVal < 0)
            {
                perror("Failed to receive value\n");
                exit(EXIT_FAILURE);
            }
            else if (recvVal == 0)
            {
                printf("[%d]: Client disconnected\n", server->clients[i]);
                close(server->clients[i]);

                server->poll_fds[i + 1] = server->poll_fds[server->clientID];
                server->clients[i] = server->clients[i - 1];
                server->clientID--;
            }
            else
            {
                server->buffer[recvVal] = '\0';
                printf("Received from [%d]: %s\n", server->clients[i], server->buffer);
            }
        }
    }
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


void    waitForEvents(s_server *server)
{
    // while(1)
    // {
        if (poll(server->poll_fds, server->clientID + 1, -1) == -1)
        {
            perror("Poll error\n");
            exit(EXIT_FAILURE);
        }
        //check for new connection
        if(server->poll_fds[0].revents & POLLIN)
        {
            server->cl_addrLen = sizeof(server->cl_addr);
            int new_client = Accept(server->master_socket, (struct sockaddr *)&(server->cl_addr), &(server->cl_addrLen));
            //setnonblock
            int flag = fcntl(new_client, F_GETFL, 0);
            flag |= O_NONBLOCK;
            fcntl(new_client, F_SETFL, flag);

            if(server->clientID < MAX_CLIENTS)
            {
                server->clients[server->clientID++] = new_client;
                server->poll_fds[server->clientID].fd = new_client;
                server->poll_fds[server->clientID].events = POLLIN | POLLOUT;
                server->poll_fds[server->clientID].revents = 0;
                printf("New client connected [%d]:\n", new_client);

            }
            else
            {
                perror("Error: Reached maximum clients\n");
                close(new_client);
            }
        }
    // }
}

void    closeAll(s_server *server)
{
    if(server->master_socket != 0)
    {
        close(server->master_socket);
        server->master_socket = 0;
    }

    for(int i = 0; i < MAX_CLIENTS; i++)
    {
        if(server->poll_fds[i].fd != 0)
        {
            close(server->poll_fds[i].fd);
            server->poll_fds[i].fd = 0;
        }
    }
}