#ifndef SERVER_H
#define SERVER_H

#include <netdb.h>
#include <stdint.h>

#define CLIENT_BUFFER_SIZE 4096

typedef struct Server
{
    int32_t fd;
    const char *port;
    struct addrinfo *res;
} Server;

Server *create_server(const char *port);
void free_server(Server *server);

int32_t start_server(Server *server);
void close_server(Server *server);

/*
 * function gets client_fd as input
 */
int32_t handle_client_loop(Server *server, void (*function)(int32_t));

#endif
