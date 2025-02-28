#ifndef SERVER_H
#define SERVER_H

#include <netdb.h>
#include <stdint.h>

typedef struct Server {
    int32_t fd;
    const char *port;
    struct addrinfo *res;
} Server;

Server *create_server(const char *port);
void free_server(Server *server);

int32_t start_server(Server *server);
void close_server(Server *server);

#endif
