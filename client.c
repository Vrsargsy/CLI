#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "get_next_line.h"

#define BUFFER_SIZE 1024
#define MAX_CMD_SIZE 3

// void cmd_connect(char* msg)
// {

// }
// void cmd_disconnect(char *msg)
// {
// }
// void cmd_shell(char *msg)
// {
// }

// void    client_command_handler(char* buff)
// {
//     char *cmd = strtok(buff, " ");

//     if (!strcmp(cmd, "connect"))
//     {
//         cmd_connect();
//         return;
//     }
//     if (!strcmp(cmd, "disconnect"))
//     {
//         cmd_disconnect();
//         return;
//     }
//     if(!strcmp(cmd, "shell"))
//     {
//         cmd_shell();
//         return;
//     }

// }

int main(int ac, char **av)
{
    if (ac != 3)
    {
        printf("ok\n");
        perror("Invalid arguments\n");
        exit(-1);
    }

    /*
        (Domain) AF_INET is IPv4
        (Socket type) SOCK_STREAM is TCP
        (Protocol) 0 kernel will decide which protocol will be better
    */
    int clientSocket_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serverAddress;
    /*
        sockaddr_in.sin_family is Domain
        sockaddr_in.sin_port is Port
        sockaddr_in.sin_addr.s_addr is MASK
        ~ INADDR_ANY is 0.0.0.0 All localhost addresses
        ~ INADDR_LOOPBACK is Loopback interface address 127.0.0.1
        ~ INADDR_BROADCAST is Broadcast address 255.255.255.255
    */
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(atoi(av[2]));
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (connect(clientSocket_fd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) != 0)
    {
        perror("Error: Connect error\n");
        exit(-1);
    }

    while (1)
    {
        printf("Client> ");
        char *buff = get_next_line(0);
        if (send(clientSocket_fd, (struct sockaddr *)&serverAddress, sizeof(serverAddress), 0) < 0)
        {
            perror("Error: can't send\n");
            exit(-1);
        }
        free(buff);
    }
    return (0);
}