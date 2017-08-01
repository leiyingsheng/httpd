#include "map.h"
#include <string.h>
#include <stdlib.h>
// #include <stdio.h>

static unsigned int hash(char* str) {
  unsigned int val;

  for (val = 0; *str != '\0'; ++str) {
    val = *str + 31 * val;
  }

  return val % HASH_SIZE;
};

struct map* newMap() {
  struct map* p = (struct map*)malloc(sizeof(struct map));
  p->entryTab = (struct entry**)malloc(sizeof(struct entry) * HASH_SIZE);

  if (NULL == p->entryTab) {
    return NULL;
  }

  return p;
}

static struct entry* lookup(struct map* caller, char* key) {
  struct entry* p;

  for (p = caller->entryTab[hash(key)]; p != NULL; p = p->next) {
    if (strcmp(key, p->key) == 0) {
      return p;
    }
  }

  return NULL;
};

int map_set(struct map* caller, char* key, void* val) {
  struct entry* p;
  unsigned int hashval;

  if (NULL == (p = lookup(caller, key))) {
    p = (struct entry*)malloc(sizeof(struct entry));
    if (NULL == p) {
      return -1;
    }

    p->key = key;
    hashval = hash(key);
    p->next = caller->entryTab[hashval];
    caller->entryTab[hashval] = p;

  } else {
    // free(p->val);
  }

  p->val = val;

  return 0;
};

void* map_get(struct map* caller, char* key) {
  struct entry* p = lookup(caller, key);

  if (NULL == p) {
    return NULL;
  }

  return p->val;
}

int map_clean(struct map* caller) {
  int i;
  struct entry *p, *tmp;

  for (i = 0; i < HASH_SIZE; ++i) {
    p = caller->entryTab[i];
    while (p != NULL) {  // clean up the list
      tmp = p;
      p = p->next;
      free(tmp);
    }
    caller->entryTab[i] = NULL;
  }

  free(caller->entryTab);
  // caller->entryTab = NULL;

  return 0;
}