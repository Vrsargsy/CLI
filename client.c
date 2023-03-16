#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define GREEN "\e[01;32m"
#define RED   "\033[0;31m"
#define RESET "\033[0m"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int client_socket, valread;
    struct sockaddr_in address;
    char buffer[BUFFER_SIZE] = {0};
    
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
    
    // main loop
    while (1) {
        // read input from the user
        printf(GREEN"<CLIENT> ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strlen(buffer) - 1] = '\0';
        
        // send the message to the server
        send(client_socket, buffer, strlen(buffer), 0);
        
        // receive a response from the server
        valread = read(client_socket, buffer, BUFFER_SIZE);
        if(valread == 0)
            continue;
        printf("             %s<SERVER>\n%s %s\n", RED, buffer, RESET);
    }
    
    return 0;
}