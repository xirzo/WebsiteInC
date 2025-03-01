#include "server.h"

#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_PENDING_CON 10

Server* create_server(const char* port, Routes* routes) {
    Server* s = malloc(sizeof(Server));

    s->fd = -1;
    s->port = port;
    s->routes = routes;

    return s;
}

void free_server(Server* server) {
    free(server);
}

int32_t start_server(Server* server) {
    struct addrinfo hints;

    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int32_t addrinfo_r;

    if ((addrinfo_r = getaddrinfo(NULL, server->port, &hints, &server->res)) != 0) {
        printf("Server getaddrinfo error: %s\n", strerror(errno));
        return -1;
    }

    if ((server->fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("Server socket error: %s\n", strerror(errno));
        return -1;
    }

    int32_t b = 0;

    int opt = 1;

    if (setsockopt(server->fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        printf("Server setsockopt error: %s\n", strerror(errno));
        return -1;
    }

    if ((b = bind(server->fd, server->res->ai_addr, server->res->ai_addrlen)) == -1) {
        printf("Server bind error: %s\n", strerror(errno));
        return -1;
    }

    int32_t l = 0;

    if ((l = listen(server->fd, MAX_PENDING_CON)) == -1) {
        printf("Server listen error: %s\n", strerror(errno));
        return -1;
    }

    printf("Server is listening on port: %s\n", server->port);

    return 0;
}

void close_server(Server* server) {
    if (server->fd == -1) {
        printf("Server fd is not initalized to be closed");
        return;
    }

    close(server->fd);
    server->fd = -1;

    if (server->res == NULL) {
        printf("Server res is not initalized to be freed");
        return;
    }

    freeaddrinfo(server->res);
}

int32_t handle_client_loop(Server* server, void (*function)(Server*, int32_t)) {
    while (1) {
        struct sockaddr_storage client_addr;
        socklen_t client_addrlen = sizeof(client_addr);
        int client_fd;

        if ((client_fd = accept(server->fd, (struct sockaddr*)&client_addr,
                                &client_addrlen)) == -1) {
            printf("Server accept error: %s\n", strerror(errno));
            return -1;
        }

        printf("Client connected.\n");

        (*function)(server, client_fd);

        close(client_fd);
    }

    close_server(server);
    return 0;
}
