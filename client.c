#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <readline/history.h>
#include <readline/readline.h>

#define GREEN "\e[01;32m"
#define RED   "\033[0;31m"
#define RESET "\033[0m"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345
#define BUFFER_SIZE 1024


//make buffer dynamic.


int main(int argc, char *argv[])
{
    struct sockaddr_in address;
    int client_socket;
    ssize_t recVal;

    char    *readBuff = (char*)malloc(sizeof(BUFFER_SIZE) + 1);
    char    *bigData = (char*)malloc(sizeof(BUFFER_SIZE) + 1);
    char    *rl_buff = NULL;
    readBuff[BUFFER_SIZE] = '\0';
    bigData[BUFFER_SIZE] = '\0';

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
        rl_buff = readline("<CLIENT> ");
        if(rl_buff)
            add_history(rl_buff);
        else
            return (0);
        if (*rl_buff)
        {
            send(client_socket, rl_buff, strlen(rl_buff), 0);
            /*
            while ((nDataLength = recv(Socket, buffer, sizeof(buffer), 0)) > 0) {
                myString.append(buffer, nDataLength);
            }
            */

            while( (recVal = recv(client_socket, readBuff, BUFFER_SIZE, 0) ) > 0)
            {
                strncat(bigData, readBuff,recVal);
                if(recVal > 1024)
                {
                    if( (realloc(bigData, BUFFER_SIZE) ) != NULL)
                        continue ;
                    else
                    {
                        perror("Failed to allocate memory\n");
                        exit(EXIT_FAILURE);
                    }
                }
                puts(bigData);
                printf("len: %lu\n", strlen(bigData));
            }
            puts(bigData);
            free(bigData);
            bigData = NULL;
            bzero(rl_buff, strlen(rl_buff));
            bzero(readBuff, strlen(readBuff));
        }
    }

    return 0;
}