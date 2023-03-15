#include "client_utils.h"

void    initClientStructure(s_client *client, char *address, char *port)
{
    client->addr.sin_port = htons(atoi(port));
    client->addr.sin_family = AF_INET;
    inet_pton(AF_INET, address, &(client->addr.sin_addr.s_addr));
}

s_client *allocateAndInitClient(char *av1, char *av2)
{
    s_client *client = (s_client*)malloc(sizeof(s_client));
    if(!client)
    {
        perror("Failed to allocate client\n");
        exit(EXIT_FAILURE);
    }
    client->sockfd = Socket(AF_INET, SOCK_STREAM, 0);
    initClientStructure(client, av1, av2);
    client->addrLen = (socklen_t)sizeof(client->addr);
    client->isConnected = 0;
    return (client);
}

void    closeAndFree(s_client *client)
{
    free(client->buffer);
    client->buffer = NULL;
    close(client->sockfd);
}

void    argCheck(int ac)
{
    if (ac != 3)
    {
        printf("ok\n");
        perror("Invalid arguments\n");
        exit(-1);
    }
}

void    handleClientCommand(s_client *client)
{

    printf("Please type 'connect' to connect to the server\n");
    char *cmd = NULL;

    while(1)
    {
        cmd = get_next_line(0); 
        if(strcmp(cmd, "disconnect") == 0)
        {
            if(client->isConnected == 1)
            {
                Send(client->sockfd, client->buffer, 0, 0);
                client->isConnected = 0;
            }
            else
            {
                perror("Failed to disconnect, you are not connected !\n");
            }
        }
        else if(strcmp(cmd, "connect") == 0)
        {
            if(client->isConnected == 1)
            {
                perror("Failed to connect, you are connected !\n");
            }
            else
            {
                Connect(client->sockfd, (struct sockaddr *)&client->addr, client->addrLen);
                client->isConnected = 1;
                free(cmd);
                cmd = NULL;
                return ;
            }
        }
         // handle shell with tok
        else
        {
            perror("Error: Type 'connect' to connect to the server\n");
        }
        free(cmd);
        cmd = NULL;
    }
}

void    clientRecv(s_client *client)
{
    bzero(client->recvBuffer, BUFFER_SIZE);

    if((recv(client->sockfd, client->recvBuffer, BUFFER_SIZE, 0)) < 0)
    {
        perror("Failed to receive\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        recv(client->sockfd, client->recvBuffer, BUFFER_SIZE, 0);
        printf("%s ", client->recvBuffer);
        // puts("TEST\n");
        return ;
    }
}
// POL
void    runChat(s_client *client)
{
    while(1)
    {
        client->buffer = get_next_line(0);
        client->buffer[BUFFER_SIZE] = '\0';
        
        Send(client->sockfd, client->buffer, BUFFER_SIZE, 0);
        free(client->buffer);
        clientRecv(client);
        client->buffer = NULL;
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
