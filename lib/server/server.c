#include "server.h"

#include <errno.h>
#include <http_parser.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_PENDING_CON 10

typedef struct {
  const char *extension;
  const char *content_type;
} ContentTypeMapping;

ContentTypeMapping content_type_table[] = {
    {"jar", "application/java-archive"},
    {"x12", "application/EDI-X12"},
    {"edi", "application/EDIFACT"},
    {"js", "application/javascript"},
    {"bin", "application/octet-stream"},
    {"ogg", "application/ogg"},
    {"pdf", "application/pdf"},
    {"xhtml", "application/xhtml+xml"},
    {"swf", "application/x-shockwave-flash"},
    {"json", "application/json"},
    {"jsonld", "application/ld+json"},
    {"xml", "application/xml"},
    {"zip", "application/zip"},
    {"form", "application/x-www-form-urlencoded"},

    {"mp3", "audio/mpeg"},
    {"wma", "audio/x-ms-wma"},
    {"ra", "audio/vnd.rn-realaudio"},
    {"wav", "audio/x-wav"},

    {"gif", "image/gif"},
    {"jpeg", "image/jpeg"},
    {"jpg", "image/jpeg"},
    {"png", "image/png"},
    {"tiff", "image/tiff"},
    {"ico", "image/vnd.microsoft.icon"},
    {"icon", "image/x-icon"},
    {"djvu", "image/vnd.djvu"},
    {"svg", "image/svg+xml"},

    {"mixed", "multipart/mixed"},
    {"alternative", "multipart/alternative"},
    {"related", "multipart/related"},
    {"form-data", "multipart/form-data"},

    {"css", "text/css"},
    {"csv", "text/csv"},
    {"event-stream", "text/event-stream"},
    {"html", "text/html"},
    {"htm", "text/html"},
    {"js", "text/javascript"},
    {"txt", "text/plain"},
    {"xml", "text/xml"},

    {"mpeg", "video/mpeg"},
    {"mp4", "video/mp4"},
    {"mov", "video/quicktime"},
    {"wmv", "video/x-ms-wmv"},
    {"avi", "video/x-msvideo"},
    {"flv", "video/x-flv"},
    {"webm", "video/webm"},

    {"apk", "application/vnd.android.package-archive"},
    {"odt", "application/vnd.oasis.opendocument.text"},
    {"ods", "application/vnd.oasis.opendocument.spreadsheet"},
    {"odp", "application/vnd.oasis.opendocument.presentation"},
    {"odg", "application/vnd.oasis.opendocument.graphics"},
    {"xls", "application/vnd.ms-excel"},
    {"xlsx",
     "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"},
    {"ppt", "application/vnd.ms-powerpoint"},
    {"pptx", "application/"
             "vnd.openxmlformats-officedocument.presentationml.presentation"},
    {"doc", "application/msword"},
    {"docx",
     "application/vnd.openxmlformats-officedocument.wordprocessingml.document"},
    {"xul", "application/vnd.mozilla.xul+xml"},

    {NULL, "text/plain"}};

const char *getContentType(const char *extension) {
  size_t i = 0;

  while (content_type_table[i].extension != NULL) {
    if (strcmp(extension, content_type_table[i].extension) == 0) {
      return content_type_table[i].content_type;
    }

    i++;
  }

  return content_type_table[i].content_type;
}

Server *createServer(const char *port, Routes *routes) {
  Server *s = malloc(sizeof(Server));

  s->fd = -1;
  s->port = port;
  s->routes = routes;

  return s;
}

Server *createServerWithDomain(const char *hostname, const char *port,
                               Routes *routes) {
  Server *s = malloc(sizeof(Server));

  s->fd = -1;
  s->port = port;
  s->hostname = hostname;
  s->routes = routes;

  return s;
}

void freeServer(Server *server) { free(server); }

int32_t startServer(Server *server) {
  struct addrinfo hints;

  memset(&hints, 0, sizeof(hints));

  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  int32_t addrinfo_r;

  if ((addrinfo_r = getaddrinfo(server->hostname ? server->hostname : NULL,
                                server->port, &hints, &server->res)) != 0) {
    fprintf(stderr, "Server getaddrinfo error: %s\n", strerror(errno));
    return -1;
  }

  if ((server->fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    fprintf(stderr, "Server socket error: %s\n", strerror(errno));
    return -1;
  }

  int32_t b = 0;

  int opt = 1;

  if (setsockopt(server->fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    fprintf(stderr, "Server setsockopt error: %s\n", strerror(errno));
    return -1;
  }

  if ((b = bind(server->fd, server->res->ai_addr, server->res->ai_addrlen)) ==
      -1) {
    fprintf(stderr, "Server bind error: %s\n", strerror(errno));
    return -1;
  }

  int32_t l = 0;

  if ((l = listen(server->fd, MAX_PENDING_CON)) == -1) {
    fprintf(stderr, "Server listen error: %s\n", strerror(errno));
    return -1;
  }

  fprintf(stderr, "Server is listening on port: %s\n", server->port);

  return 0;
}

void defaultClientLoop(Server *s, int32_t client_fd) {
  char client_buffer[CLIENT_BUFFER_SIZE] = {0};

  ssize_t value_read = read(client_fd, client_buffer, CLIENT_BUFFER_SIZE - 1);

  if (value_read <= 0) {
    fprintf(stderr, "Client disconnected or read error: %s\n", strerror(errno));
    return;
  }

  if (value_read >= CLIENT_BUFFER_SIZE - 1) {
    fprintf(stderr, "Client data exceeds buffer size. Truncating.\n");
    client_buffer[CLIENT_BUFFER_SIZE - 1] = '\0';
  }

  HttpRequest *r;
  initHttpRequest(&r);
  parseRequestLine(r, client_buffer);

  char *host_header = strstr(client_buffer, "Host:");
  char host[256] = {0};

  if (host_header) {
    sscanf(host_header + 5, "%255s", host);
    char *port_separator = strchr(host, ':');
    if (port_separator) {
      *port_separator = '\0';
    }

    fprintf(stderr, "Request for host: %s\n", host);
  }

  char *key = malloc(strlen(r->uri) + 1);

  strcpy(key, r->uri + 1);

  char *filename = getRoute(s->routes, key);

  FILE *fptr = fopen(filename, "rb");

  if (fptr == NULL) {
    fprintf(stderr, "There is no file to read from\n");
    free(key);
    freeHttpRequest(r);
    return;
  }

  fseek(fptr, 0, SEEK_END);
  int64_t file_size = ftell(fptr);
  fseek(fptr, 0, SEEK_SET);

  char *body = malloc(file_size);
  size_t bytes_read = fread(body, 1, file_size, fptr);

  fclose(fptr);

  if (bytes_read != file_size) {
    fprintf(stderr, "Failed to read the entire file\n");
    free(body);
    free(key);
    freeHttpRequest(r);
    return;
  }

  char *file_extension = strrchr(filename, '.');

  if (file_extension != NULL) {
    file_extension += 1;
  }

  const char *content_type = getContentType(file_extension);

  char headers[512];

  snprintf(headers, sizeof(headers),
           "HTTP/1.1 200 OK\r\n"
           "Content-Type: %s\r\n"
           "Content-Length: %ld\r\n"
           "Connection: close\r\n"
           "\r\n",
           content_type, file_size);

  send(client_fd, headers, strlen(headers), 0);
  send(client_fd, body, file_size, 0);

  free(key);
  freeHttpRequest(r);
}

int32_t startServerWithDefaultLoop(Server *server) {
  startServer(server);
  updateClientLoop(server, defaultClientLoop);
  return 0;
}

void closeServer(Server *server) {
  if (server->fd == -1) {
    fprintf(stderr, "Server fd is not initalized to be closed");
    return;
  }

  close(server->fd);
  server->fd = -1;

  if (server->res == NULL) {
    fprintf(stderr, "Server res is not initalized to be freed");
    return;
  }

  freeaddrinfo(server->res);
}

int32_t updateClientLoop(Server *server, void (*function)(Server *, int32_t)) {
  while (1) {
    struct sockaddr_storage client_addr;
    socklen_t client_addrlen = sizeof(client_addr);
    int client_fd;

    if ((client_fd = accept(server->fd, (struct sockaddr *)&client_addr,
                            &client_addrlen)) == -1) {
      fprintf(stderr, "Server accept error: %s\n", strerror(errno));
      return -1;
    }

    fprintf(stderr, "Client connected.\n");

    (*function)(server, client_fd);

    close(client_fd);
  }

  closeServer(server);
  return 0;
}
