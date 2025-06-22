#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "routes.h"
#include "server.h"

#define MAX_PENDING_CONNECTIONS 10
#define MAX_NUMBER_OF_ROUTES 10
#define MAX_REQUEST_SIZE 16 * 1024 * 1024

// TODO: Parse body on HttpParser

int main(int argc, char *argv[]) {
  const char *PORT = getenv("PORT");

  if (!PORT || strlen(PORT) == 0) {
    fprintf(stderr,
            "You did not specify the \"PORT\" environmental variable\n");
    return 1;
  }

  Routes *r = createRoutes(MAX_NUMBER_OF_ROUTES);

  insertRoute(r, "", "index.html");
  insertRoute(r, "style.css", "style.css");
  insertRoute(r, "favicon.ico", "favicon.ico");

  insertRoute(r, "console", "favicon.ico");

  Server *s = createServer(PORT, MAX_PENDING_CONNECTIONS, MAX_REQUEST_SIZE, r);

  if (startServer(s) != 0) {
    fprintf(stderr, "error: Failed to start server\n");
    freeServer(s);
    return 1;
  }

  acceptClientConnection(s);

  closeServer(s);
  freeServer(s);
  return 0;
}
