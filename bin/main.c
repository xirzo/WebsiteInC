#include <stdlib.h>
#include <unistd.h>

#include "server.h"

int main(int argc, char *argv[]) {
    Routes *routes = malloc(sizeof(*routes));

    insertRoute(routes, "", "index.html");
    insertRoute(routes, "style.css", "style.css");

    Server *s = createServer("5000", routes);

    startServerWithDefaultLoop(s);

    closeServer(s);

    return EXIT_SUCCESS;
}
