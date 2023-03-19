#include "client.h"

static int mat_len(char **mat)
{
    int i = 0;
    while (mat[i])
        i++;
    return i;
}

int main(void)
{
    struct sockaddr_in address;
    int client_socket;
    char    SERVER_IP[] = "127.0.0.1";
    char     receiveBuffSize[BUFFER_SIZE + 1] = {0};
    char    *bigData  = NULL;
    char    *rl_buff = NULL;
    char    *sendSizeToServer = NULL;
    char    **splitted_rl = NULL;
    int     is_connected = 0;
    receiveBuffSize[BUFFER_SIZE] = '\0';

    // create a TCP socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    while (1)
    {
        rl_buff = readline(GREEN"<CLIENT> "RESET);
        if(*rl_buff)
        {
            splitted_rl = split(rl_buff, ' ');
            if(!splitted_rl || mat_len(splitted_rl) != 3)
            {
                printf("bad arguments\n");
                continue;
            }
            if( strcmp(splitted_rl[0], "connect") == 0)
            {
                // set up the server address
                address.sin_family = AF_INET;
                address.sin_addr.s_addr = inet_addr(splitted_rl[1]);
                address.sin_port = htons(atoi(splitted_rl[2]));
    
                // connect to the server
                if (connect(client_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
                {
                    perror("connect failed");
                    continue;
                }
                else
                {
                    printf("Connected to server %s:%s\n", SERVER_IP, splitted_rl[2]);
                    is_connected = 1;
                    freeSplittedArr(splitted_rl);
                    break ;
                }
            }
        }
        
    }
    while(1)
    {
        rl_buff = readline(GREEN"<CLIENT> "RESET);
        if(rl_buff)
            add_history(rl_buff);
        else
            return (0);
        if (*rl_buff)
        {            
            if(strcmp(rl_buff,"disconnect") == 0 && is_connected == 1)
            {
                close(client_socket);
            }
   
            else
            {
                if ( (send(client_socket,rl_buff, strlen(rl_buff), 0) ) < 0)   
                {
                    perror("Failed to send message \n");
                    continue;
                }
                /* Receive from server  */
                else
                {
                    if( (recv(client_socket, receiveBuffSize, BUFFER_SIZE, 0) ) > 0)
                    {
                        bigData = (char *)malloc(sizeof(char) * (atoi(receiveBuffSize) + 1));
                        if (!bigData)
                        {
                            perror("Failed to allocate the memory: \n");
                            exit(EXIT_FAILURE);
                        }
                        bigData[atoi(receiveBuffSize)] = '\0';
                        if ( (recv(client_socket, bigData, atoi(receiveBuffSize), MSG_WAITALL) ) > 0)
                        {
                            printf(RED "            SERVER\n%s\n"RESET, bigData);
                        }
                        free(bigData);
                        free(sendSizeToServer);
                        bigData = NULL;
                        sendSizeToServer = NULL;
                        bzero(receiveBuffSize, strlen(receiveBuffSize));
                    }
                }
            }
        }
        else
            continue ;
    }
    return 0;
}