#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "server.h"

int main(void) {
  Routes *routes = malloc(sizeof(*routes));
  if (!routes) {
    fprintf(stderr, "Failed to allocate memory for routes\n");
    return EXIT_FAILURE;
  }
  routes->size = 0;

  const char *port = getenv("port");

  if (!port || strlen(port) == 0) {
    fprintf(stderr,
            "You did not specify the \"port\" environmental variable\n");
    free(routes);
    return EXIT_FAILURE;
  }

  insertRoute(routes, "", "/app/assets/index.html");
  insertRoute(routes, "style.css", "/app/assets/style.css");
  insertRoute(routes, "favicon.ico", "/app/assets/favicon.ico");

  Server *s = createServer(port, routes);
  if (!s) {
    fprintf(stderr, "Failed to create server\n");
    free(routes);
    return EXIT_FAILURE;
  }

  if (startServerWithDefaultLoop(s) != 0) {
    fprintf(stderr, "Failed to start server on port %s\n", port);
    closeServer(s);
    freeServer(s);
    free(routes);
    return EXIT_FAILURE;
  }

  freeRoutes(routes);
  closeServer(s);
  freeServer(s);
  free(routes);

  return EXIT_SUCCESS;
}
