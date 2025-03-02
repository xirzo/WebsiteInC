#include "server.h"

#include <errno.h>
#include <http_parser.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "file_reader.h"

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

void default_client_loop(Server* s, int32_t client_fd) {
    char client_buffer[CLIENT_BUFFER_SIZE] = {0};
    ssize_t value_read = read(client_fd, client_buffer, CLIENT_BUFFER_SIZE - 1);

    if (value_read <= 0) {
        printf("Client disconnected or read error: %s\n", strerror(errno));
        return;
    }

    if (value_read >= CLIENT_BUFFER_SIZE - 1) {
        printf("Client data exceeds buffer size. Truncating.\n");
        client_buffer[CLIENT_BUFFER_SIZE - 1] = '\0';
    }

    HttpRequest* r;

    init_http_request(&r);

    parse_request_line(r, client_buffer);

    char* key = malloc(sizeof(r->uri));

    strcpy(key, r->uri + 1);

    printf("%s\n", key);

    char* filename = get_route(s->routes, key);

    FILE* fptr = fopen(filename, "r");

    if (fptr == NULL) {
        printf("There is no file to read from\n");
        return;
    }

    char* html_body = read_file(fptr);
    fclose(fptr);

    char headers[512];
    int content_length = strlen(html_body);
    snprintf(headers, sizeof(headers),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: text/html\r\n"
             "Content-Length: %d\r\n"
             "Connection: close\r\n"
             "\r\n",
             content_length);

    send(client_fd, headers, strlen(headers), 0);
    send(client_fd, html_body, content_length, 0);

    free_http_request(r);
    free(key);
}

int32_t start_server_with_default_loop(Server* server) {
    start_server(server);
    handle_client_loop(server, default_client_loop);
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
