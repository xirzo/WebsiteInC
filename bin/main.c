#include <stdlib.h>
#include <unistd.h>

#include "server.h"

int main(int argc, char *argv[]) {
    Routes *routes = malloc(sizeof(*routes));

    insert_route(routes, "", "index.html");
    insert_route(routes, "style.css", "style.css");

    Server *s = create_server("5000", routes);

    start_server_with_default_loop(s);

    close_server(s);

    return EXIT_SUCCESS;
}
