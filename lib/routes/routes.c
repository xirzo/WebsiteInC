#include "routes.h"
#include <stdio.h>

int32_t getRoutesIndex(Routes *routes, const char key[]) {
  for (size_t i = 0; i < routes->size; i++) {
    if (strcmp(routes->keys[i], key) == 0) {
      return i;
    }
  }
  return -1;
}

void insertRoute(Routes *routes, const char key[], const char value[]) {
  int index = getRoutesIndex(routes, key);

  if (index == -1) {
    if (routes->size >= MAX_NUMBER_OF_ROUTES) {
      fprintf(stderr, "Max number of routes exceeded\n");
      return;
    }

    strncpy(routes->keys[routes->size], key, MAX_KEY_LENGTH - 1);
    routes->keys[routes->size][MAX_KEY_LENGTH - 1] = '\0';

    routes->values[routes->size] = strdup(value);
    if (routes->values[routes->size] == NULL) {
      fprintf(stderr, "Memory allocation failed for route value\n");
      return;
    }
    routes->size++;
  } else {
    free(routes->values[index]);
    routes->values[index] = strdup(value);
    if (routes->values[index] == NULL) {
      fprintf(stderr, "Memory allocation failed for route value\n");
    }
  }
}

char *getRoute(Routes *routes, const char key[]) {
  int32_t index = getRoutesIndex(routes, key);

  if (index == -1) {
    return NULL;
  } else {
    return routes->values[index];
  }
}

void freeRoutes(Routes *routes) {
  for (size_t i = 0; i < routes->size; i++) {
    free(routes->values[i]);
  }
  routes->size = 0;
}
