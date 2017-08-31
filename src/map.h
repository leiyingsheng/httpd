#pragma once

#include "utils.h"

#define HASH_SIZE 50

/* list entry */
struct entry {
  char* key;
  void* val;
  struct entry* next;
};

struct Map {
  struct entry** entryTab;
};

struct Map* newMap();

/* val must be malloced */
int setMap(struct Map* m, char* key, void* val);

void* getMap(struct Map* m, char* key);

int cleanMap(struct Map* m);

int printMap(struct Map* m);
