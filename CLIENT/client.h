#ifndef CLIENT_H
# define CLIENT_H

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

#endif