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

Server *create_server(const char *port, Routes *routes);
void free_server(Server *server);

int32_t start_server(Server *server);
int32_t start_server_with_default_loop(Server *server);
void close_server(Server *server);

int32_t handle_client_loop(Server *server, void (*function)(Server *, int32_t));

#endif
