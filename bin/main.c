#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "server.h"

int main(void) {
    Routes *routes = malloc(sizeof(*routes));

    const char *port = getenv("port");

    if (port == NULL || strlen(port) == 0) {
        printf("You did not specifty \"port\" environmental variable\n");
        return EXIT_FAILURE;
    }

    insertRoute(routes, "", "index.html");
    insertRoute(routes, "style.css", "style.css");
    insertRoute(routes, "favicon.ico", "favicon.ico");
    Server *s = createServer(port, routes);

    startServerWithDefaultLoop(s);

    closeServer(s);

    return EXIT_SUCCESS;
}
