#pragma once

#include "utils.h"

#define HASH_SIZE 50

/* list entry */
struct entry {
  char* key;
  char* val;
  struct entry* next;
};

struct map {
  struct entry** entryTab;
};

struct map* newMap();

int setMap(struct map* m, char* key, char* val);

char* getMap(struct map* m, char* key);

int cleanMap(struct map* m);

int printMap(struct map* m);
