#include "general.h"
#include "server_utils.h"

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

void Recv(s_server *server)
{

    for(int i = 0; i < server->clientID; i++)
    {
        if(server->poll_fds[i+1].revents & POLLIN)
        {
            ssize_t recvVal = recv(server->clients[i], server->buffer, BUFFER_SIZE, 0);
            if(recvVal < 0)
            {
                perror("Failed to receive value\n");
                // exit(EXIT_FAILURE);
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
                //command handler
                handleCommand(server, i);
               // if (){}
               // printf("Received from [%d]: %s\n", server->clients[i], server->buffer);
            }
        }
    }
}

void    waitForEvents(s_server *server)
{
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

void    servAcCheck(int ac)
{
    if (ac != 2)
    {
        perror("Invalid arguments: Arguments must be 2\n");
        exit(EXIT_FAILURE);
    }    
}

s_server    *createServer(char *av1)
{
    s_server *server = (s_server*)malloc(sizeof(s_server));
    server->addrLen = (socklen_t)sizeof(server->addr);
    server->master_socket = Socket(AF_INET, SOCK_STREAM, 0); // Create socket
    int flags = fcntl(server->master_socket,F_GETFL, 0);
    flags |= O_NONBLOCK;
    fcntl(server->master_socket, F_SETFL, flags);
    initServerStructure(server, av1);

    return (server);

}

void    runServer(s_server *server)
{
    while(1)
    {
        waitForEvents(server);
        Recv(server);
    }
}

// Server Handler connect disconnect shell:

void    serverHandler(s_server *server, int i)
{
    char    errorMsg[50] = "You are connected now. You can't connect again.\n";
    if(strcmp(server->buffer, "disconnect") == 0)
    {
        close(server->clients[i]);
        server->poll_fds[i + 1] = server->poll_fds[server->clientID];
        server->clients[i] = server->clients[i - 1];
        server->clientID--;
        printf("[%d]: Client disconnected\n", server->clients[i]);
    }
    else
    {
        Send(server->clients[i], &errorMsg, 50, 0);
    }
}

void    handleCommand(s_server *server, int i)
{
    char    **buff = split(server->buffer, ' ');

    if (!buff)
    {
        puts("buff is empty\n");
        return ;
    }
    if ((strcmp(buff[0], "disconnect") == 0 || strcmp(buff[0], "connect") == 0) && buff[1] == NULL)
    {
        serverHandler(server, i);
    }
    else if (strcmp(buff[0], "shell") == 0)
    {
        if(!buff[1])
        {
            perror("Error: command is empty. Example: shell \"ls -la\" \n");
            free(buff[0]);
            return ;
        }
        else if (buff[1] && buff[2] == NULL)
        {
            executeCommand(server, i);
            return ;
        }
        else
        {
            perror("Error: Invalid input. Example: shell \"ls -la\" \n");
            return ;
        }
    }
    else
    {
        printf("Received from [%d]: %s\n", server->clients[i], server->buffer);
    }

    // for(int i = 0; i <)
    // buff = NULL;
    
    return ;
}

void    executeCommand(s_server *server, int i) //shell "ls" 
{
    FILE    *output = NULL;
    char    **tok = split(server->buffer, ' ');
    printf("TOK 1:%s  2:%s\n", tok[0], tok[1]);
    char    errorMsg[] = "Failed to execute command\n";
    //char    buffer[BUFFER_SIZE + 1];
    //buffer[BUFFER_SIZE] = '\0';
    
    output = popen(tok[1], "r+");
    // puts("ERROR HANDLE\n");
    if (output == NULL)
    {
        Send(server->clients[i], &errorMsg, strlen(errorMsg), 0);
    }
    else
    {
        while (fgets(server->buffer,BUFFER_SIZE,output) != NULL)
        {
           //int rd = read((int)output, server->buffer, BUFFER_SIZE);
           // if(rd <= 0)
            //    break ;
            Send(server->clients[i], server->buffer, BUFFER_SIZE, 0);
            //bzero(server->buffer, BUFFER_SIZE);
        }
        /* int rd = read(output, server->buffer, BUFFER_SIZE);
        if(rd <= 0)
            break ;
        Send(server->clients[i], server->buffer, BUFFER_SIZE, 0);
        bzero(server->buffer, BUFFER_SIZE); */
        
        
    }
    pclose(output);
    output = NULL;
}