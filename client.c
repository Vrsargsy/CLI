#include "client_utils.h"

int main(int ac, char **av)
{
    argCheck(ac);
    s_client *client = allocateAndInitClient(av[1],av[2]);
    handleCommand(client);    
    runChat(client);
    closeAndFree(client);

    return (0);
}