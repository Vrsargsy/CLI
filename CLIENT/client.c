#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <readline/history.h>
#include <readline/readline.h>

#include "../SERVER/server.h"

#define GREEN "\e[01;32m"
#define RED   "\033[0;31m"
#define RESET "\033[0m"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345


//make buffer dynamic.


int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    struct sockaddr_in address;
    int client_socket;

    char     receiveBuffSize[BUFFER_SIZE + 1] = {0};
    char    *bigData  = NULL;
    char    *rl_buff = NULL;
    char    *sendSizeToServer = NULL;
    
    receiveBuffSize[BUFFER_SIZE] = '\0';

    // create a TCP socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // set up the server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(SERVER_IP);
    address.sin_port = htons(SERVER_PORT);
    
    // connect to the server
    if (connect(client_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }
    
    printf("Connected to server %s:%d\n", SERVER_IP, SERVER_PORT);
    
    while(1)
    {
        rl_buff = readline(GREEN"<CLIENT> "RESET);
        if(rl_buff)
            add_history(rl_buff);
        else
            return (0);
        if (*rl_buff)
        {
            /*  Send data size to server */
            sendSizeToServer = itoa(strlen(rl_buff));
            if ( (send(client_socket,sendSizeToServer,strlen(sendSizeToServer), 0) ) < 0)   
            {
                perror("Failed to send size of message \n");
                exit(EXIT_FAILURE);
            }
            else
            {
                usleep(500);
                if ( (send(client_socket,rl_buff, atoll(sendSizeToServer), 0) ) < 0)   
                {
                    perror("Failed to send message \n");
                    exit(EXIT_FAILURE);
                }
            }
            /* Receive from server  */
            if ( (recv(client_socket, receiveBuffSize, BUFFER_SIZE, 0) ) > 0)
            {
                bigData = (char *)malloc(sizeof(char) * atoi(receiveBuffSize) + 1);
                if (!bigData)
                {
                    perror("Failed to allocate the memory: \n");
                    exit(EXIT_FAILURE);
                }
                else
                {
                    bigData[atoi(receiveBuffSize)] = '\0';
                     if ( (recv(client_socket, receiveBuffSize, BUFFER_SIZE, 0) ) > 0)
                    printf(RED "            SERVER\n%s\n"RESET, bigData);

                    free(bigData);
                    free(sendSizeToServer);

                    bigData = NULL;
                    sendSizeToServer = NULL;
                }
            }
        }
    }
    return 0;
}