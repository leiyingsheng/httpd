#pragma once
#define HASH_SIZE 50

// static struct entry* entryTab[HASH_SIZE];

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

static struct entry* lookup(struct map* caller, char* key);

int map_set(struct map* caller, char* key, void* val);

void* map_get(struct map* caller, char* key);

int map_clean(struct map* caller);