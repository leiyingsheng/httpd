#pragma once

#define HASH_SIZE 50

/* list entry */
struct entry {
  char* key;
  void* val;
  struct entry* next;
};

struct map {
  struct entry** entryTab;
};

struct map* newMap();

static unsigned int hash(char* str);

static struct entry* lookup(struct map* m, char* key);

int setMap(struct map* m, char* key, char* val);

char* getMap(struct map* m, char* key);

int cleanMap(struct map* m);

int printMap(struct map* m);