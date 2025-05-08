#ifndef ROUTES_H
#define ROUTES_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUMBER_OF_ROUTES 100
#define MAX_KEY_LENGTH 100

typedef struct Routes {
  size_t size;
  char keys[MAX_NUMBER_OF_ROUTES][MAX_KEY_LENGTH];
  char *values[MAX_NUMBER_OF_ROUTES];
} Routes;

int32_t getRoutesIndex(Routes *routes, const char key[]);
void insertRoute(Routes *routes, const char key[], const char value[]);
char *getRoute(Routes *routes, const char key[]);
void freeRoutes(Routes *routes);

#endif
