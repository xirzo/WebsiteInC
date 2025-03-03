#ifndef SERVER_H
#define SERVER_H

#include <netdb.h>
#include <stdint.h>

#include "routes.h"

#define CLIENT_BUFFER_SIZE 4096

typedef struct Server
{
    int32_t fd;
    const char *port;
    struct addrinfo *res;
    Routes *routes;
} Server;

Server *createServer(const char *port, Routes *routes);
void freeServer(Server *server);

int32_t startServer(Server *server);
int32_t startServerWithDefaultLoop(Server *server);
void closeServer(Server *server);

int32_t updateClientLoop(Server *server, void (*function)(Server *, int32_t));

#endif
