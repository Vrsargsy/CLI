#include "handling.h"
#include "get_next_line.h"

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

    s_client *client = NULL;

    client = (s_client*)malloc(sizeof(s_client));

    client->sockfd = Socket(AF_INET, SOCK_STREAM, 0);
    initClientStructure(client, av[1], av[2]);
    client->addrLen = (socklen_t)sizeof(client->addr);
    Connect(client->sockfd, (struct sockaddr *)&client->addr, client->addrLen);
    
    while(1)
    {
        client->buffer = get_next_line(0);
        Send(client->sockfd, client->buffer, BUFFER_SIZE, 0);
        free(client->buffer);
    }


    close(client->sockfd);

}