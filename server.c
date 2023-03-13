#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <errno.h>

#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024

typedef struct t_server
{
    int option;
    int socketFD;
    int new_socketFD;
    int clientSocket[MAX_CLIENTS];
    int activity;
    int valReaded;
    int sd;
    int max_sd;
    int addrLen;
    int nb;
    char buffer[BUFFER_SIZE + 1];

    struct sockaddr_in address;
    fd_set readFds;
} s_server;

void initializeClientSockets(s_server *server)
{
    for (server->nb = 0; server->nb < MAX_CLIENTS; server->nb++)
        server->clientSocket[server->nb] = 0;
}

void createSocketFD(s_server *server)
{
   	if ((server->socketFD = socket(AF_INET, SOCK_STREAM, 0)) < 0)/* if ((server->socketFD = socket(AF_INET, SOCK_STREAM, 0) < 0)) */
    {
        perror("Failed to create socket\n");
        exit(-1);
    }
    else
        printf("Socket Created!\n");
}

void setServerSockOption(s_server *server)
{
    server->option = 1;

    if (setsockopt(server->socketFD, SOL_SOCKET, SO_REUSEADDR, (char *)&(server->option), sizeof(server->option)) < 0)/* if (setsockopt(server->socketFD, SOL_SOCKET, SO_REUSEADDR, (char *)&(server->option), sizeof(server->option) < 0)) */
    {
        perror("Failed to set server option\n");
        exit(-1);
    }
    printf("Server option changed successfuly\n");
}

void createSocketType(s_server *server, char *port)
{
    server->address.sin_family = AF_INET;
    server->address.sin_port = htons(atoi(port));
    server->address.sin_addr.s_addr = INADDR_ANY;
}

void bindSocket(s_server *server)
{
    // printf("%ld\n", sizeof(server->address));
   	if (bind(server->socketFD, (struct sockaddr *)&(server->address), sizeof(server->address)) < 0)/* if (bind(server->socketFD, (struct sockaddr *)&(server->address), sizeof(&server->address)) < 0) */
    {
        perror("Failed to bind sockets\n");
        exit(-1);
    }
    printf("Server Binded!");
}

void startListening(s_server *server)
{
    if (listen(server->socketFD, MAX_CLIENTS) < 0)
    {
        perror("Failed to listen\n");
        exit(-1);
    }
    printf("Server in listening mode now\n");

    while (1)
    {
        FD_ZERO(&(server->readFds));
        FD_SET(server->socketFD, &(server->readFds));
        server->max_sd = server->socketFD;

        for (server->nb = 0; server->nb < MAX_CLIENTS; server->nb++)
        {
            server->sd = server->clientSocket[server->nb];

            if (server->sd > 0)
                FD_SET(server->sd, &(server->readFds));
            if (server->sd > 0)
                server->max_sd = server->sd;
        }
    }
}

void addNewSocket(s_server *server)
{
    for (server->nb = 0; server->nb < MAX_CLIENTS; server->nb++)
    {
        if (server->clientSocket[server->nb] == 0)
        {
            server->clientSocket[server->nb] = server->new_socketFD;
            printf("New Socket Added!\n");
            break;
        }
    }
}
void waitForActivity(s_server *server)
{
    server->activity = select(server->max_sd + 1, &(server->readFds), NULL, NULL, NULL);

    if ((server->activity < 0) && (errno != EINTR))
    {
        perror("Failed in selection\n");
    }
    server->addrLen = sizeof(server->address);
    if (FD_ISSET(server->socketFD, &(server->readFds)))
	{
   		 if ((server->new_socketFD = accept(server->socketFD, (struct sockaddr *)&server->address, (socklen_t *)&(server->addrLen))) < 0)
    	{
        	perror("Accept\n");
       		 exit(-1);
   	 	}
    	addNewSocket(server);
	}
}


void recieveCommand(s_server *server)
{
    if (FD_ISSET(server->sd, &(server->readFds)))
    {
		printf("ok\n");
        if ((server->valReaded = recv(server->sd, server->buffer, BUFFER_SIZE, 0)) == 0)
        {
            getpeername(server->sd, (struct sockaddr *)&(server->address), (socklen_t *)&(server->addrLen));
            printf("~Disconnected IP: %s Port: %d\n", inet_ntoa(server->address.sin_addr), ntohs(server->address.sin_port));

            close(server->sd);
            server->clientSocket[server->nb] = 0;
        }
        else
        {
            server->buffer[server->valReaded] = '\0';
            printf("Message recieved!\n");
            // handle command
        }
    }
}

int main(int ac, char **av)
{
    if (ac != 2)
    {
        perror("Invalid arguments\n");
        exit(-1);
    }

    s_server *server = (s_server *)malloc(sizeof(s_server));

    initializeClientSockets(server);
    createSocketFD(server);
    createSocketType(server, av[1]);
    bindSocket(server);
    startListening(server);
    waitForActivity(server);
    addNewSocket(server);
    recieveCommand(server);

    return (0);
}
