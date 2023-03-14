#include "server_utils.h"
#include "general.h"


int main(int ac, char **av)
{
    servAcCheck(ac);
    s_server *server = createServer(av[1]);
    Bind(server->master_socket, (struct sockaddr *)&(server->addr), server->addrLen); // Bind sockets
    Listen(server->master_socket, MAX_CLIENTS);
    runServer(server);
    closeAll(server);
}