#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>
#include <fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include "../HELPERS/helpers.h"


# define GREEN "\e[01;32m"
# define RED   "\033[0;31m"
# define RESET "\033[0m"

# define MAX_CLIENTS 5
# define BUFFER_SIZE 1024

char	*exec(char *command);

#endif