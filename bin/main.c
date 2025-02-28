#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

int main(int argc, char *argv[]) {
    Server *s = create_server(PORT);

    start_server(s);

    size_t current_client = -1;

    FILE *fptr = fopen(FILENAME, "r");

    if (fptr == NULL) {
        printf("There is no file to read from\n");
        return EXIT_FAILURE;
    }

    char *html_body = read_file(fptr);
    fclose(fptr);

    while (1) {
        struct sockaddr_storage client_addr;
        socklen_t client_addrlen = sizeof(client_addr);
        int client_fd;

        if ((client_fd =
                 accept(s->fd, (struct sockaddr *)&client_addr, &client_addrlen)) == -1) {
            printf("Server accept error: %s\n", strerror(errno));
            return -1;
        }

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

        printf("Client connected.\n");

        close(client_fd);
    }

    close_server(s);

    return EXIT_SUCCESS;
}
