#ifndef SERVER_UTILS_H
# define SERVER_UTILS_H

#include "general.h"
#include <fcntl.h>

void        initServerStructure(s_server *server, char *port);
void        Listen(int sockfd, int backlog);
void        waitForEvents(s_server *server);
void        closeAll(s_server *server);
void        Recv(s_server *server);
void        servAcCheck(int ac);
void        runServer(s_server *server);
s_server    *createServer(char *av1);

#endif