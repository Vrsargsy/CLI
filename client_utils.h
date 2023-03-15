#ifndef CLIENT_UTILS_H
# define CLIENT_UTILS_H

#include "general.h"
#include "get_next_line.h"

void        runChat(s_client *client);
void        handleClientCommand(s_client *client);
s_client    *allocateAndInitClient(char *av1, char *av2);
void        argCheck(int ac);
void        closeAndFree(s_client *client);
void        Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
void    	initClientStructure(s_client *client, char *address, char *port);
void    	clientRecv(s_client *client);

#endif