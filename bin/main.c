#include <errno.h>
#include <http_parser.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "router.h"
#include "server.h"

#define PORT "5000"
#define FILENAME "index.html"

char *read_file(FILE *f) {
    if (f == NULL || fseek(f, 0, SEEK_END)) {
        return NULL;
    }

    long length = ftell(f);

    rewind(f);

    if (length == -1 || (unsigned long)length >= SIZE_MAX) {
        return NULL;
    }

    size_t ulength = (size_t)length;

    char *buffer = malloc(ulength + 1);

    if (buffer == NULL || fread(buffer, 1, ulength, f) != ulength) {
        free(buffer);
        return NULL;
    }

    buffer[ulength] = '\0';

    return buffer;
}

void client_handling_loop(Server *s, int32_t client_fd) {
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

    printf("Raw data received: %s\n", client_buffer);
    printf("%zd\n", value_read);

    HttpRequest *r;

    init_http_request(&r);

    parse_request_line(r, client_buffer);

    char *key = malloc(sizeof(r->uri));

    strcpy(key, r->uri + 1);

    printf("%s\n", key);

    char *filename = get_route(s->routes, key);

    FILE *fptr = fopen(filename, "r");

    if (fptr == NULL) {
        printf("There is no file to read from\n");
        return;
    }

    char *html_body = read_file(fptr);
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

int main(int argc, char *argv[]) {
    Routes *routes = malloc(sizeof(*routes));

    insert_route(routes, "home", "home.html");
    insert_route(routes, "blog", "blog.html");

    Server *s = create_server(PORT, routes);

    start_server(s);

    size_t current_client = -1;

    ssize_t value_read;
    char client_buffer[CLIENT_BUFFER_SIZE] = {0};

    handle_client_loop(s, client_handling_loop);

    close_server(s);

    return EXIT_SUCCESS;
}
