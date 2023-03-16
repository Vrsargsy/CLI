#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>

# define BUFFER_SIZE 1024
# define GREEN "\e[01;32m"
# define RED   "\033[0;31m"
# define RESET "\033[0m"

char *exec(char *command) {
   char buffer[BUFFER_SIZE + 1];
   char *result = malloc(BUFFER_SIZE);
   unsigned int  i = 0;
   unsigned int  ri = 0;
   bzero(buffer, strlen(buffer));
   bzero(result, BUFFER_SIZE);
   // Open pipe to file
   FILE* pipe = popen(command, "r");
   if (!pipe) {
      return "popen failed!";
   }
   buffer[BUFFER_SIZE] = '\0';

    int cnt = 0;
   // read till end of process:
        while ( (buffer[i] = fgetc(pipe)) != EOF)
        {
            result[ri] = buffer[i];
            i++;
            ri++;
            if (i = BUFFER_SIZE)
            {
                bzero(buffer, strlen(buffer));
                i = 0;
            }
        }
    result[ri] = '\0';
    pclose(pipe);
    return result;
}

#define MAX_CLIENTS 10

int main(int argc, char *argv[]) {
    int server_socket, new_socket, i, activity, addrlen, valread;
    int client_sockets[MAX_CLIENTS] = {0};
    struct sockaddr_in address;
    char buffer[BUFFER_SIZE] = {0};
    struct pollfd fds[MAX_CLIENTS + 1];
    char    *toClient = NULL;
    // create a TCP socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // set socket options
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }
    
    // bind the socket to an IP address and port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(12345);
    if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    // listen for incoming connections
    if (listen(server_socket, 3) < 0) {
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
        for (i = 0; i < MAX_CLIENTS; i++) {
            if (client_sockets[i] > 0 && fds[i + 1].revents & POLLIN) {
                valread = read(client_sockets[i], buffer, BUFFER_SIZE);
            
            if (valread == 0) {
                // client disconnected
                printf("%sClient[%d] disconnected%s\n", RED, client_sockets[i+1],RESET);
                close(client_sockets[i]);
                client_sockets[i] = 0;
                fds[i + 1].fd = -1;
            } else {
                // process incoming data
                printf("%sClient[%d]: %s%s\n", RED, i + 1, buffer,RESET);
                toClient = exec(buffer);
                // echo the data back to the client
                send(client_sockets[i], toClient, strlen(toClient), 0);
                free(toClient);
                toClient = NULL;
                bzero(buffer, strlen(buffer));
            }
        }
    }
    }
    return 0;
}

