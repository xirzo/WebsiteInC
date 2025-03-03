#ifndef ROUTES_H
#define ROUTES_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define MAX_NUMBER_OF_ROUTES 100

typedef struct Routes
{
    size_t size;
    char keys[MAX_NUMBER_OF_ROUTES][100];
    char *values[MAX_NUMBER_OF_ROUTES];
} Routes;

int32_t getRoutesIndex(Routes *routes, char key[]);
void insertRoute(Routes *routes, char key[], char value[]);
char *getRoute(Routes *routes, char key[]);

#endif
