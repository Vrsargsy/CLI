#include "./server.h"
int main(int argc, char *argv[])
{
    (void)argv;
    if (argc != 2)
    {
        exit(-1);
    }
    int server_socket, new_socket, i, activity, addrlen, valread;
    int client_sockets[MAX_CLIENTS] = {0};
    struct sockaddr_in address;
    struct pollfd fds[MAX_CLIENTS + 1];

    char     buffer[BUFFER_SIZE + 1] = {0};
    char    *toClient   = NULL;
    char    *sendSize   = NULL;

    // create a TCP socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // set socket options
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }
    
    // bind the socket to an IP address and port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(atoi(argv[1]));
    if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    // listen for incoming connections
    if (listen(server_socket, MAX_CLIENTS) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    
    printf("%sListening for connections on port %d%s\n", RED,ntohs(address.sin_port),RESET);
    
    // set up the pollfd structures
    memset(fds, 0, sizeof(fds));
    fds[0].fd = server_socket;
    fds[0].events = POLLIN;
    
    // main loop
    while (1) {
        // call poll()
        activity = poll(fds, MAX_CLIENTS + 1, -1);
        if (activity < 0) {
            perror("poll failed");
            exit(EXIT_FAILURE);
        }
        
        // check for incoming connections
        if (fds[0].revents & POLLIN) {
            if ((new_socket = accept(server_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
                perror("accept failed");
                exit(EXIT_FAILURE);
            }
            
            printf("New connection from %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
            
            // add the new socket to the list of client sockets
            for (i = 0; i < MAX_CLIENTS; i++) {
                if (client_sockets[i] == 0) {
                    client_sockets[i] = new_socket;
                    fds[i + 1].fd = new_socket;
                    fds[i + 1].events = POLLIN;
                    break;
                }
            }
        }
        
        // check for incoming data from clients
        for (i = 0; i < MAX_CLIENTS; i++)
        {
            if (client_sockets[i] > 0 && fds[i + 1].revents & POLLIN)
            {
                valread = recv(client_sockets[i], buffer, BUFFER_SIZE, 0);
                if (valread == 0)
                {
                    // client disconnected
                    printf("%sClient[%d] disconnected%s\n", RED, client_sockets[i+1],RESET);
                    close(client_sockets[i]);
                    client_sockets[i] = 0;
                    fds[i + 1].fd = -1;
                }
                else if(valread < 0)
                {
                    perror("Failed to receive message\n");
                }
                else
                {
                    printf("%sClient[%d]: %s%s\n", RED, i + 1, buffer,RESET);
                    if (strcmp(buffer, "disconnect") == 0)
                    {
                        close(client_sockets[i]);
                        bzero(buffer, strlen(buffer));
                        printf("%sClient[%d] disconnected%s\n", RED, client_sockets[i+1],RESET);
                        continue ;
                    }
                    else if(strncmp(buffer,"connect", 7) == 0)
                    {
                        bzero(buffer, strlen(buffer));
                        continue;
                    }
                    toClient = exec(buffer);
                    puts(toClient);
                    if (toClient)
                    {
                        sendSize = itoa(strlen(toClient));
                        send(client_sockets[i],sendSize,strlen(sendSize), 0);
                        usleep(500);
                        send(client_sockets[i], toClient, strlen(toClient), 0);
                        free(toClient);
                        free(sendSize);
                        sendSize = NULL;
                        toClient = NULL;
                    }
                    bzero(buffer, strlen(buffer));
                }
            }
        }
    }
    return 0;
}
