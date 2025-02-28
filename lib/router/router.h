#ifndef ROUTER_H
#define ROUTER_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define MAX_NUMBER_OF_ROUTES 100

typedef struct Routes {
    size_t size;
    char keys[MAX_NUMBER_OF_ROUTES][100];
    int values[MAX_NUMBER_OF_ROUTES];
} Routes;

int32_t get_routes_index(Routes *routes, char key[]);
void insert_route(Routes *routes, char key[], int value);
int32_t get_route(Routes *routes, char key[]);

#endif
